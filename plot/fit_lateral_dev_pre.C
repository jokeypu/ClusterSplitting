PndEmcMapper *emcMap;
PndEmcRecoPar *fRecoPar;
double TotEnergy(const TClonesArray* fHitArray);
int IndexOfMax(const TClonesArray* fHitArray);
TVector3 LinearWhere(const TClonesArray* fHitArray);
TVector3 LiloWhere(const TClonesArray* fHitArray, std::vector<Double_t> params);
TVector3 Where(const TClonesArray* fHitArray);
int Exec(const TString my_dir_name);

int fit_lateral_dev_pre(){
    PndEmcMapper::Init(1);
    TString emcParFile      = gSystem->Getenv("MY_PAR_FILE");
    TString emcAsciiFile    = gSystem->Getenv("MY_ASCII_FILE");
    FairRunAna *fRun= new FairRunAna();
    FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
    emcMap = PndEmcMapper::Instance();
    FairParRootFileIo* parInput1 = new FairParRootFileIo();
    parInput1->open(emcParFile.Data());
    FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
    parIo1->open(emcAsciiFile.Data(),"in");
    rtdb->setFirstInput(parInput1);
    rtdb->setSecondInput(parIo1);
    fRecoPar = (PndEmcRecoPar *)(rtdb->getContainer("PndEmcRecoPar"));
    TFile *parfile = new TFile(emcParFile);
    parfile->Get("FairGeoParSet");
    rtdb->initContainers(0);

    if (Exec("photon_0.2GeV")) return 1;
    if (Exec("photon_0.3GeV")) return 1;
    if (Exec("photon_0.4GeV")) return 1;
    if (Exec("photon_0.5GeV")) return 1;
    if (Exec("photon_0.6GeV")) return 1;
    if (Exec("photon_0.7GeV")) return 1;
    if (Exec("photon_0.8GeV")) return 1;
    if (Exec("photon_0.9GeV")) return 1;
    if (Exec("photon_1.0GeV")) return 1;
    if (Exec("photon_1.5GeV")) return 1;
    if (Exec("photon_2.0GeV")) return 1;
    if (Exec("photon_2.5GeV")) return 1;
    if (Exec("photon_3.0GeV")) return 1;
    if (Exec("photon_3.5GeV")) return 1;
    if (Exec("photon_4.0GeV")) return 1;
    if (Exec("photon_4.5GeV")) return 1;
    if (Exec("photon_5.0GeV")) return 1;
    if (Exec("photon_5.5GeV")) return 1;
    if (Exec("photon_6.0GeV")) return 1;

    return 0;
}

double TotEnergy(const TClonesArray* fHitArray){
 double total = .0;
 int nhits = fHitArray->GetEntriesFast();
 for (int i = 0; i < nhits; i++) {
                PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
                total += hit->GetEnergy();
        }
 return total;
}

int IndexOfMax(const TClonesArray* fHitArray){
 double max = .0;
 int index_max = -1;
 int nhits = fHitArray->GetEntriesFast();
 for (int i = 0; i < nhits; i++) {
                PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
                Double_t e = hit->GetEnergy();
                if ( e > max ) { max = e; index_max = i; }
        }
 return index_max;
}

TVector3 LinearWhere(const TClonesArray* fHitArray)
{
  TVector3 Error(0,0,0); 
  const double lClusEnergy = TotEnergy(fHitArray);
  int nhits = fHitArray->GetEntriesFast();
  assert(lClusEnergy != 0);
  TVector3 lLinSum(0, 0, 0);
  PndEmcTciXtalMap const &tciXtalMap = PndEmcStructure::Instance()->GetTciXtalMap();
for (int i = 0; i < nhits; i++) {
    PndEmcHit* lDigi = (PndEmcHit*)fHitArray->At(i);
    TVector3 lDigiWhere;
    lDigi->Position(lDigiWhere);
    const double lDigiEnergy = lDigi->GetEnergy();
    const double lLinWeight = lDigiEnergy / lClusEnergy;
    lLinSum += lLinWeight * lDigiWhere;
  }

  PndEmcTwoCoordIndex *lTCI = PndEmcStructure::Instance()->locateIndex(lLinSum.Theta(), lLinSum.Phi());
  assert(lTCI != 0);
  const PndEmcXtal *lGeom = tciXtalMap.find(lTCI)->second;
  if (lGeom == NULL) return Error;
  const TVector3 lCentre = lGeom->frontCentre();
  const TVector3 lNormal = lGeom->normalToFrontFace();
  const TVector3 lVector = lCentre - lLinSum;
  const double lLength = lNormal.Dot(lVector);

  if (lLength < 0.0) {
  const TVector3 lUnit = lLinSum.Unit();
    const double lMag = (lNormal.Dot(lCentre) / lNormal.Dot(lUnit));
    lLinSum.SetMag(lMag);
  }

  return lLinSum;
}

TVector3 LiloWhere(const TClonesArray* fHitArray, std::vector<Double_t> params)
{
  TVector3 Error(0,0,0);
  PndEmcMapper *emcMap = PndEmcMapper::Instance();

  Double_t offsetParmA = params[0];
  Double_t offsetParmB = params[1];
  Double_t offsetParmC = params[2];

  const double lClusEnergy = TotEnergy(fHitArray);

  assert(lClusEnergy != 0);

  const double lOffset = offsetParmA - offsetParmB * exp(-offsetParmC * pow(lClusEnergy, 1.171)) * pow(lClusEnergy, -0.534);

  TVector3 lLiloPoint(1, 1, 1);

  TVector3 lLinSum(0, 0, 0);
  TVector3 lLogSum(0, 0, 0);

  double lLogWeightSum = 0;
  int lLogNum = 0;

  bool lLogSecondTheta = false;
  bool lLogSecondPhi = false;
  int lLogFirstTheta = -666;
  int lLogFirstPhi = -666;

  PndEmcTciXtalMap const &tciXtalMap = PndEmcStructure::Instance()->GetTciXtalMap();

int nhits = fHitArray->GetEntriesFast();
for (int i = 0; i < nhits; i++) {
    PndEmcHit* lDigi = (PndEmcHit*)fHitArray->At(i);
    PndEmcTwoCoordIndex *tci = emcMap->GetTCI(lDigi->GetDetectorID());
    PndEmcXtal *xtal = tciXtalMap.find(tci)->second;
    if (xtal == NULL) return Error;
    const TVector3 tNormal = xtal->normalToFrontFace();
    TVector3 lDigiWhere;
    lDigi->Position(lDigiWhere);

    const int lDigiTheta = tci->XCoord();
    const int lDigiPhi = tci->YCoord();
    const double lDigiEnergy = lDigi->GetEnergy();
    const double lLinWeight = lDigiEnergy / lClusEnergy;
    const double lLogWeight = lOffset + log(lLinWeight);

    lLinSum += lLinWeight * lDigiWhere;
    if (lLogWeight > 0) {
      lLogSum += lLogWeight * lDigiWhere;
      lLogWeightSum += lLogWeight;
      lLogNum++;

      if (lLogNum == 1) {
        lLogFirstTheta = lDigiTheta;
        lLogFirstPhi = lDigiPhi;
      } else {
        if (!lLogSecondTheta && lDigiTheta != lLogFirstTheta)
          lLogSecondTheta = true;
        if (!lLogSecondPhi && lDigiPhi != lLogFirstPhi)
          lLogSecondPhi = true;
      }
    }
  }

  if (lLogNum > 0)
    lLogSum *= 1. / lLogWeightSum;

  lLiloPoint.SetTheta(lLogSecondTheta ? lLogSum.Theta() : lLinSum.Theta());
  lLiloPoint.SetPhi(lLogSecondPhi ? lLogSum.Phi() : lLinSum.Phi());

  PndEmcTwoCoordIndex *lTCI = PndEmcStructure::Instance()->locateIndex(lLiloPoint.Theta(), lLiloPoint.Phi());

  assert(lTCI != 0);

  const PndEmcXtal *lGeom = tciXtalMap.find(lTCI)->second;
  if (lGeom == NULL) return Error;
  const TVector3 lLiloVector = lLiloPoint;
  const TVector3 lCentre = lGeom->frontCentre();

  const TVector3 lNormal = lGeom->normalToFrontFace();
  const TVector3 lVector = lCentre - lLiloVector;
  const double lLength = lNormal.Dot(lVector);
  if (lLength < 0.0) {
  const TVector3 lUnit = lLiloVector.Unit();
    const double lMag = (lNormal.Dot(lCentre) / lNormal.Dot(lUnit));
    lLiloPoint.SetMag(lMag);
  } else {
    if (lLogNum > 1) {
      // Use logarithmic centroid position
      lLiloPoint.SetMag(lLogSum.Mag());
      // cout<<" With log centroid method, lilo-position "<<lLiloPoint.Mag()<<endl;
      } else {
      lLiloPoint.SetMag(lLinSum.Mag());
      }
  }

  return lLiloPoint;
}

TVector3 Where(const TClonesArray* fHitArray){
    TVector3 pos;
    std::vector<Double_t> fClusterPosParam;
    TString method = fRecoPar->GetEmcClusterPosMethod();
    if (!strcmp(method, "lilo")) {
    	//cout << "Lilo cluster position method" << endl;
    	fClusterPosParam.push_back(fRecoPar->GetOffsetParmA());
    	fClusterPosParam.push_back(fRecoPar->GetOffsetParmB());
    	fClusterPosParam.push_back(fRecoPar->GetOffsetParmC());
        pos = LiloWhere(fHitArray, fClusterPosParam);
    } else if (!strcmp(method, "linear")) {
        pos = LinearWhere(fHitArray);
    } else {
        std::cout << "Incorrect cluster position method " << std::endl;
        abort();
    }
    return pos;
}
    
int Exec(const TString my_dir_name){
    TString data_dir = gSystem->Getenv("PND_DATA_DIR");
    TString my_file = data_dir + "/" + my_dir_name  + "/sim/sim_emc_*";
    TChain *chain = new TChain("pndsim");
    chain->Add(my_file);

    TFile f("doc/fit_lateral_dev.root","update");
    TTree mytree(my_dir_name, "lateral_development");
    Double_t Energy, r, theta;
    mytree.Branch("Energy",&Energy,"Energy/D");
    mytree.Branch("r",&r,"r/D");
    mytree.Branch("theta",&theta,"theta/D");

    TClonesArray* fMCTrackArray = new TClonesArray("PndMCTrack");
    chain->SetBranchAddress("MCTrack",&fMCTrackArray);
    if (!fMCTrackArray) return -1;

    TClonesArray* fHitArray = new TClonesArray("PndEmcHit");
    chain->SetBranchAddress("EmcHit",&fHitArray);
    if (!fHitArray) return -1;

    Int_t maxEvtNo = chain->GetEntries();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        if (maxEvtNo>=100 && ievt%(maxEvtNo/100)==0) cout << 100 * (int)ievt/maxEvtNo << "%" << endl;
        chain->GetEntry(ievt);
        int nhits = fHitArray->GetEntriesFast();
        
        PndMCTrack *mcTrack = (PndMCTrack *)fMCTrackArray->At(0);
        TVector3 mom = mcTrack->GetMomentum();
        if ( TotEnergy(fHitArray)/mom.Mag() < 0.6 ) continue;
        theta = mom.Theta();

        TVector3 aVector = Where(fHitArray);
        if (aVector.Mag() < 0.1) continue;
       
        PndEmcHit* hit_max = (PndEmcHit*)fHitArray->At(IndexOfMax(fHitArray));
        TVector3 pos_max(0, 0, 0);
        hit_max->Position(pos_max);
        Double_t r_max = pos_max.Mag()*TMath::Sin(aVector.Angle(pos_max));
        if ( r_max > 1.7 ) continue; //1.5

        for (int i = 0; i < nhits; i++) {
		PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
		TVector3 pos(0, 0, 0);
                hit->Position(pos);
		if ( hit->GetModule() != 1 && hit->GetModule() != 2 ) continue;
		r = pos.Mag()*TMath::Sin(aVector.Angle(pos));
		Energy =  hit->GetEnergy();
                if (r < 1 && Energy/mom.Mag() < 0.2) cout << "+" << endl;
		mytree.Fill();
        }
    }
    mytree.Write();
    delete chain;
    delete fMCTrackArray;
    delete fHitArray;
    f.Close();
    return 0;
}
