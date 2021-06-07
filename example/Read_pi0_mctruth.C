int Read_pi0_mctruth( TString DataDir = "pi0_test_6.0GeV" ){
    TCanvas* c1=new TCanvas("c1","PANDA",800,600);
    gStyle->SetOptTitle(0);
    gStyle->SetStatX(0.36);
    gStyle->SetStatY(0.88);
    gStyle->SetOptStat(1);
    gStyle->SetLabelFont(42,"xyz");
    gStyle->SetLabelSize(0.06,"xyz");
    gStyle->SetLabelOffset(0.01,"xyz");
    gStyle->SetNdivisions(510,"xyz");
    gStyle->SetTitleFont(42,"xyz");
    gStyle->SetTitleColor(1,"xyz");
    gStyle->SetTitleSize(0.06,"xyz");
    gStyle->SetTitleOffset(1.2,"xyz");
    
    c1->SetLeftMargin(0.14);
    c1->SetRightMargin(0.1);
    c1->SetTopMargin(0.08);
    c1->SetBottomMargin(0.17);
    
    TH1D * h1 = new TH1D("Histogram", "h1", 100, 0, 5);
    h1->GetYaxis()->SetTitle("Entries");
    h1->GetXaxis()->SetTitle("E_{Bump}   [GeV]");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    h1->SetLineColor(kBlue);
    h1->SetLineWidth(2);
    
    FairRunAna *fRun = new FairRunAna();
    TString DataName = "sim_emc_0.root";//定义文件名
    TString emcDataFile = gSystem->Getenv("PND_DATA_DIR");//获取环境变量
    emcDataFile = emcDataFile + "/" + DataDir + "/sim/" + DataName;//*1*组合为文件路径
    //emcDataFile = "/scratchfs/bes/puqing/pnd_data/test/reco_emc.root"; //*2*直接输入文件路径
    TChain *chain = new TChain("pndsim");
    chain->Add(emcDataFile);

    TClonesArray* fMCTrackArray = new TClonesArray("PndMCTrack");
    chain->SetBranchAddress("MCTrack",&fMCTrackArray);
    if (!fMCTrackArray) return -1;

    TClonesArray* fHitArray = new TClonesArray("PndEmcHit");
    chain->SetBranchAddress("EmcHit",&fHitArray);
    if (!fHitArray) return -1;
    
    int maxEvtNo = chain->GetEntries();
    for (unsigned ievt = 0; ievt < maxEvtNo; ievt++) {
        if (maxEvtNo>=100 && ievt%(maxEvtNo/100)==0) cout << 100 * (int)ievt/maxEvtNo << "%" << endl;
        chain->GetEntry(ievt);
        int nhits = fHitArray->GetEntriesFast();
        std::map<Int_t, Double_t> bump_truth;

        for (unsigned ihit = 0; ihit < nhits ; ihit++){
	    PndEmcHit* hit = (PndEmcHit*)fHitArray->At(ihit);
	    std::map<Int_t, Double_t> itruth_map = hit->GetDepositedEnergyMap(); 
            for (std::map<Int_t, Double_t>::iterator iter = itruth_map.begin(); iter != itruth_map.end(); iter++){
                PndMCTrack *mcTrack = (PndMCTrack*)fMCTrackArray->At(iter->first);   
                if ( mcTrack->GetMotherID() != 0 ) continue;
                if ( bump_truth.find(iter->first) == bump_truth.end() ) bump_truth.insert(std::map<Int_t, Double_t>::value_type(iter->first, iter->second));
		else bump_truth[iter->first] += iter->second;
	    }
 	}
        if ( bump_truth.size()!=2 ) continue;

        for (std::map<Int_t, Double_t>::iterator iter = bump_truth.begin(); iter != bump_truth.end(); iter++){
            h1->Fill(iter->second);
        }
    }

    h1->Draw();
 
    TLegend * leg = new TLegend(0.69,0.72,0.88,0.85);
    leg->AddEntry(h1, "Bump Truth", "L");
    leg->SetLineColor(kWhite);
    leg->Draw("SAME");

    return 0;
}

