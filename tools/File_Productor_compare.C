Double_t DD(const TVector3 *DetPos, const TVector3 *Cent, const Double_t par){
    Double_t distance(0), angle(0);
    if (*DetPos != *Cent) {
        TVector3 vz(0, 0, 1);
        TVector3 DetPos_o = (*DetPos) - 3.7*vz;
        TVector3 DetPos_n;
        DetPos_n.SetMagThetaPhi(DetPos_o.Mag(), DetPos_o.Theta(), DetPos_o.Phi()-0.06981317);
        TVector3 ey = DetPos_n.Cross(vz).Unit();
        TVector3 ex = DetPos_n.Cross(ey).Unit();
        Double_t dx = abs((*Cent-*DetPos).Dot(ex));
        Double_t dy = abs((*Cent-*DetPos).Dot(ey));
        distance = sqrt(dx*dx+dy*dy);
        angle = 57.2957*TMath::ATan(dy/dx);
        if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
        if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    }
    return distance;
}

Double_t AA(const TVector3 *DetPos, const TVector3 *Cent, const Double_t par){
    Double_t distance(0), angle(0);
    if (*DetPos != *Cent) {
        TVector3 vz(0, 0, 1);
        TVector3 DetPos_o = (*DetPos) - 3.7*vz;
        TVector3 DetPos_n;
        DetPos_n.SetMagThetaPhi(DetPos_o.Mag(), DetPos_o.Theta(), DetPos_o.Phi()-0.06981317);
        TVector3 ey = DetPos_n.Cross(vz).Unit();
        TVector3 ex = DetPos_n.Cross(ey).Unit();
        Double_t dx = (*DetPos-*Cent).Dot(ex);
        Double_t dy = (*DetPos-*Cent).Dot(ey);
        TVector2 vv(dx,dy);
        distance = sqrt(dx*dx+dy*dy);
        angle = fabs(TMath::RadToDeg()*vv.Phi_mpi_pi(vv.Phi()));
        //if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
        //if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    }
    return angle;
}

int File_Productor_compare(std::string dir_name, Double_t Shower_Energy){
    //root File_Productor_compare.C'("WorkData_1Gamma_A12_E1.0_OR",1.0)';
    //std::string dir_name="Gamma_one_1G";
    std::string out_name = "doc/compare_"+ dir_name +".txt";
    
    std::ofstream File_out;
    File_out.open(out_name,std::ios::out);
    //File_out << "Index " << "Distance " << "Angle " << "Energy" << endl;
    
    Int_t NGamma(1);
    
    TString file_path_sim = "../data/"+dir_name+"/evtcomplete_sim.root";
    TString file_path_digi = "../data/"+dir_name+"/evtcomplete_digi.root";
    
    FairRunAna *fRun = new FairRunAna();
    TFile* file = new TFile(file_path_sim);
    FairFileSource* source = new FairFileSource(file,"InputFile");
    FairRootManager* ioman = FairRootManager::Instance();
    ioman->SetSource(source);
    ioman->InitSource();
    
    TClonesArray* fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
    if (!fMCTrackArray) return -1;
    TClonesArray* fHitArray = (TClonesArray*) ioman->GetObject("EmcHit");
    if (!fHitArray) return -1;
    
    TFile* f = new TFile(file_path_digi);
    TTree* t = (TTree*)f->Get("pndsim");
    TClonesArray* fBumpArray = new TClonesArray("PndEmcBump");
    t->SetBranchAddress("EmcBump",&fBumpArray);
    TClonesArray* fDigiArray = new TClonesArray("PndEmcDigi");
    t->SetBranchAddress("EmcDigi",&fDigiArray);
    if (!fBumpArray) return -1;
    TClonesArray* fClusterArray = new TClonesArray("PndEmcCluster");
    t->SetBranchAddress("EmcCluster",&fClusterArray);
    if (!fClusterArray) return -1;
    
    int N(0),CC(0);
    int SEED_CUT(-1);
    Int_t maxEvtNo = ioman->CheckMaxEventNo();
    //maxEvtNo /= 10;
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        ioman->ReadEvent(ievt); // read event by event
        t->GetEntry(ievt);
        if (maxEvtNo>=100 && ievt%(maxEvtNo/100)==0) cout << 100 * (int)ievt/maxEvtNo << "%" << endl;
        int nhits = fHitArray->GetEntriesFast();
        int nclusters = fClusterArray->GetEntriesFast();
        int nbumps = fBumpArray->GetEntriesFast();
        int ndigis = fDigiArray->GetEntriesFast();
        
        //if (nbumps==0) continue;
        PndEmcBump* Bump;
        Double_t Bump_E=-1;
        for (int i = 0; i < nbumps; i++){
             PndEmcBump* Bump_i = (PndEmcBump*)fBumpArray->At(i);
            if ((Bump_i->energy())>Bump_E) Bump = Bump_i;
        }
        
        bool Exist = false;
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            std::set<FairLink> links = (hit->GetTrackEntering()).GetLinks();
            for (std::set<FairLink>::iterator linkIter = links.begin(); linkIter != links.end(); linkIter++) {
                    if (linkIter->GetIndex() == 0) Exist = true;
            }
        }
        if (!Exist) continue;
        
        TVector3 Cent_pos = Bump->where();
        
        double E_max = -1.0;
        double min_distance = 99999;
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            TVector3 Det_Pos;
            hit->Position(Det_Pos);
            double Distance = DD(&Det_Pos, &Cent_pos, 1.25);
            if (Distance < min_distance) {min_distance = Distance;E_max = hit->GetEnergy();}
        }
        //cout << E_max << endl;
        if (E_max < Shower_Energy/4.0) continue;
        
        PndEmcCluster *theCluster = (PndEmcCluster *)fClusterArray->At(0);
        std::map<Int_t, Int_t> theMaximaDigis = theCluster->LocalMaxMap();
        
        std::map<Int_t, Int_t>::iterator p;
        Int_t digi_seed(-1),digi_seed_id(-1);
        int c(0);
        for (p = theMaximaDigis.begin(); p != theMaximaDigis.end(); p++){
            digi_seed = p->second;
            digi_seed_id = p->first;
            c++;
        }
        if ( c!=1 ) continue;
        
        PndEmcDigi* digi = (PndEmcDigi*)fDigiArray->At(digi_seed);
        double Seed_Energy = digi->GetEnergy();
        TVector3 Seed_pos = digi->where();
        
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
	    if (hit->GetDetectorID() == digi_seed_id) continue;
            TVector3 Det_Pos;
            hit->Position(Det_Pos);
            double Truth_Energy = hit->GetEnergy();
            double Distance = DD(&Det_Pos, &Cent_pos, 1.25);
            double dseed = DD(&Seed_pos, &Cent_pos, 1.25);
            if (Distance > 4) continue;
            File_out << dseed << " " << Distance << " " << Seed_Energy << " " << Truth_Energy << endl;
            N++;
        }
        CC++;
    }
    File_out.close();
    cout << "Max Event Nomber:" << maxEvtNo << ", " << "Passed:" << N << "   total:" << CC << endl;
    return 0;
}
