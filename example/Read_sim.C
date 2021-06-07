int Read_sim( TString DataDir = "test" ){
    TCanvas* c1=new TCanvas("c1","PANDA",800,600);
    gStyle->SetOptTitle(0);
    gStyle->SetStatX(0.36);
    gStyle->SetStatY(0.88);
    gStyle->SetOptStat(0);
    gStyle->SetLabelFont(42,"xyz");
    gStyle->SetLabelSize(0.06,"xyz");
    gStyle->SetLabelOffset(0.01,"xyz");
    gStyle->SetNdivisions(510,"xyz");
    gStyle->SetTitleFont(42,"xyz");
    gStyle->SetTitleColor(1,"xyz");
    gStyle->SetTitleSize(0.06,"xyz");
    gStyle->SetTitleOffset(1.2,"xyz");
    
    c1->SetLeftMargin(0.17);
    c1->SetRightMargin(0.1);
    c1->SetTopMargin(0.08);
    c1->SetBottomMargin(0.17);
    
    TH1D * h1 = new TH1D("h1", "hist", 100, 0.7, 1.4);
    h1->GetYaxis()->SetTitle("Entries");
    h1->GetXaxis()->SetTitle("Energy   [GeV]");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    h1->SetLineColor(kRed);
    h1->SetLineWidth(3);
    
    TH1D * h2 = new TH1D("h2", "hist", 100, 0.7, 1.4);
    h2->GetYaxis()->SetTitle("Entries");
    h2->GetXaxis()->SetTitle("Energy   [GeV]");
    h2->GetXaxis()->CenterTitle();
    h2->GetYaxis()->CenterTitle();
    h2->SetLineColor(kBlue);
    h2->SetLineWidth(3);

    FairRunAna *fRun = new FairRunAna();
    TString DataName = "sim_emc.root";
    TString emcDataFile = gSystem->Getenv("PND_DATA_DIR");
    emcDataFile = emcDataFile + "/" + DataDir + "/sim/" + DataName;
    TFile* file = new TFile(emcDataFile);
    FairFileSource* source = new FairFileSource(file,"InputFile");
    FairRootManager* ioman = FairRootManager::Instance();
    ioman->SetSource(source);
    ioman->InitSource();
    
    TClonesArray* fHitArray = (TClonesArray*) ioman->GetObject("EmcHit");
    if (!fHitArray) return -1;
    TClonesArray* fPointArray = (TClonesArray*) ioman->GetObject("EmcPoint");
    if (!fPointArray) return -1;

    int maxEvtNo = ioman->CheckMaxEventNo();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        ioman->ReadEvent(ievt);
        int nhits = fHitArray->GetEntriesFast();
        int npoints = fPointArray->GetEntriesFast();
        
        Double_t hit_total_energy = .0;
        Double_t point_total_energy = .0;
        
	for (int i = 0; i < nhits ; i++){
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            hit_total_energy += hit->GetEnergy();
        }
	
	for (int i = 0; i < npoints ; i++){
            PndEmcPoint* point = (PndEmcPoint*)fPointArray->At(i);
            point_total_energy += point->GetEnergyLoss();
        }
            h1->Fill(hit_total_energy);
            h2->Fill(point_total_energy);
    }

    h2->Draw();
    h1->Draw("SAME");
 
    TLegend * leg = new TLegend(0.61,0.72,0.88,0.85);
    leg->AddEntry(h1, "hit total energy", "L");
    leg->AddEntry(h2, "point total energy", "L");
    leg->Draw("SAME");

    return 0;
}

