int Exec(TString dir_name, TH2D *h, Int_t NGamma=1);
int Shower_function_hit_angle( TString dir_name="Gamma_one_1G" )
{
    int bin1(150),bin2(150),bin3(150);
    float tx(800),ty(600);
    double xmin(0),xmax(2),ymin(-2),ymax(92),zmin(0),zmax(0.1);
    
    TCanvas* c1=new TCanvas("PANDA1","c1",tx,ty);
    gStyle->SetOptTitle(0);
    gStyle->SetStatX(0.36);
    gStyle->SetStatY(0.88);
    gStyle->SetOptStat(0);
    gStyle->SetLabelFont(42,"xyz");
    gStyle->SetLabelSize(0.05,"xyz");
    gStyle->SetLabelOffset(0.01,"xyz");
    gStyle->SetNdivisions(510,"xyz");
    gStyle->SetTitleFont(42,"xyz");
    gStyle->SetTitleColor(1,"xyz");
    gStyle->SetTitleSize(0.05,"xyz");
    gStyle->SetTitleOffset(1.0,"xyz");
    
    //TH3D* h2D1 = new TH3D("Hist1","h1",bin1,xmin,xmax, bin2,ymin,ymax, bin3,zmin,zmax);
    TH2D* h2D1 = new TH2D("Hist1","h1",bin1,xmin,0.04, bin2,ymin,ymax);
    h2D1->SetMarkerStyle(7);
    h2D1->SetMarkerColorAlpha(kAzure+3, 0.5);
    h2D1->GetXaxis()->SetTitle("d(cm)");
    h2D1->GetYaxis()->SetTitle("angle");
    h2D1->GetZaxis()->SetTitle("E");
    h2D1->GetXaxis()->CenterTitle();
    h2D1->GetYaxis()->CenterTitle();
    h2D1->GetZaxis()->CenterTitle();

    if( Exec(dir_name, h2D1, 1) ) return 1;
    
    c1->cd();
    h2D1->Draw("SCAT");
    return 0;
}

//*******************************************************************************************************//
int Exec(TString dir_name, TH2D *h, Int_t NGamma){
    //NGamma: Number of photons produced
    
    TString file_path_sim = "../data/"+dir_name+"/evtcomplete_sim.root";
    
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
    TClonesArray* fPointArray = (TClonesArray*) ioman->GetObject("EmcPoint");
    if (!fPointArray) return -1;
    
    PndEmcMapper::Init(1);
    TFile *parfile = new TFile("../data/"+dir_name+"/evtcomplete_par.root");
    parfile->Get("FairGeoParSet");
    PndEmcStructure *fEmcStr = PndEmcStructure::Instance();
    PndEmcMapper *fMapper = PndEmcMapper::Instance();
    typedef std::map<Int_t, Float_t> mapper;
    mapper emcX = fEmcStr->GetEmcX();
    mapper emcY = fEmcStr->GetEmcY();
    mapper emcZ = fEmcStr->GetEmcZ();
    
    int N(0);
    const TVector3 vz(0,0,1);
    Int_t maxEvtNo = ioman->CheckMaxEventNo();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        ioman->ReadEvent(ievt); // read event by event
        int nhits = fHitArray->GetEntriesFast();
        int npoints = fPointArray->GetEntriesFast();
        
        //Get the momentum of each photon
        std::vector<TVector3> Gamma_mom;
        for (int iGamma = 0; iGamma < NGamma; iGamma++) {
            PndMCTrack *mcTrack = (PndMCTrack *)fMCTrackArray->At(iGamma);
            Gamma_mom.push_back(mcTrack->GetMomentum());
        }
        
        //Exclude events generated electron-positron
        std::map<Int_t, bool> Exist;
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            std::set<FairLink> links = (hit->GetTrackEntering()).GetLinks();
            for (std::set<FairLink>::iterator linkIter = links.begin(); linkIter != links.end(); linkIter++) {
                for (int iGamma = 0; iGamma < NGamma; iGamma++)
                    if (linkIter->GetIndex() == iGamma) Exist[iGamma] = true;
            }
        }
        if (Exist.size() != NGamma) continue;
        
        //Match hit for each photon
        PndEmcPoint* point_0 = (PndEmcPoint*)fPointArray->At(0);
        Int_t seedID = point_0->GetDetectorID();
        Int_t seedHit = -1;
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            Int_t DetID = hit->GetDetectorID();
            if (DetID == seedID) {
                seedHit = i;
                break;
            }
        }
        if (seedHit == -1) continue;
        
        // computing distance and angle from each hit to track
        for (int iGamma = 0; iGamma < NGamma; iGamma++) {
            PndEmcHit* hit0 = (PndEmcHit*)fHitArray->At(seedHit);
            Double_t E_0 = hit0->GetEnergy();
            Int_t DetID0 = hit0->GetDetectorID();
            Double_t theta = Gamma_mom[iGamma].Theta();
            Double_t R = sqrt(2*(emcX[DetID0]*emcX[DetID0] + emcY[DetID0]*emcY[DetID0])/(1-cos(2*theta)));
            TVector3 pos_in;
            pos_in.SetPtThetaPhi(R, theta, Gamma_mom[iGamma].Phi());
            TVector3 pos_n(0,1/tan(pos_in.Theta()),pos_in.Phi()); 
            for (int i = 0; i < nhits; i++) {
                PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
                Int_t DetID = hit->GetDetectorID();
                TVector3 pos(emcX[DetID], emcY[DetID], emcZ[DetID]);
                TVector3 cpos(0,1/tan(pos.Theta()),pos.Phi());
                TVector3 distance = pos - pos_in;
                Double_t d = (cpos - pos_n).Mag();
                Double_t angle = (cpos - pos_n).Angle(vz);
                angle *= 57.29578;
                if (angle>90) angle = 180 - angle;
                Double_t E = hit->GetEnergy();
                //h->Fill(E/E_0,angle);
                //h->Fill(d*cos(angle),angle,E/E_0);
                if (E == E_0) h->Fill(d,angle,E_0);
                //if ((d<1.6) && (d >1.4)) h->Fill(d,angle,E);
            }
        }
        N++;
    }
    cout << "Max Event Nomber:" << maxEvtNo << ", " << "Passed:" << N << endl;
    return 0;
}

