int Read_reco_pi0( TString DataDir = "test" ){
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
    
    TH1D * h1 = new TH1D("Histogram", "h1", 500, 50, 225);
    h1->GetYaxis()->SetTitle("Entries");
    h1->GetXaxis()->SetTitle("m_{#pi^{0}}   [MeV/c^{2}]");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    h1->SetLineColor(kBlue);
    h1->SetLineWidth(2);
    
    TCutG* cut = new TCutG("cut", 2);
    cut->SetPoint(0, 135, 0);
    cut->SetPoint(1, 135, 5000);
    cut->SetLineColor(kRed);
    cut->SetLineWidth(2);
    cut->SetLineStyle(2);

    FairRunAna *fRun = new FairRunAna();
    TString DataName = "reco_emc.root";//定义文件名
    TString emcDataFile = gSystem->Getenv("PND_DATA_DIR");//获取环境变量
    emcDataFile = emcDataFile + "/" + DataDir + "/reco/" + DataName;//*1*组合为文件路径
    //emcDataFile = "/scratchfs/bes/puqing/pnd_data/test/reco_emc.root"; //*2*直接输入文件路径
    TFile* file = new TFile(emcDataFile);
    FairFileSource* source = new FairFileSource(file,"InputFile");
    FairRootManager* ioman = FairRootManager::Instance(); //初始化RootManager
    ioman->SetSource(source);
    ioman->InitSource();
    
    TClonesArray* fBumpArray = (TClonesArray*) ioman->GetObject("EmcBump");//获取EmcBump中的数据
    if (!fBumpArray) return -1;

    int maxEvtNo = ioman->CheckMaxEventNo();
    for (unsigned ievt = 0; ievt < maxEvtNo; ievt++) {
        ioman->ReadEvent(ievt);
        int nbumps = fBumpArray->GetEntriesFast();
        
        std::vector <TLorentzVector*> All_P; //设置存放两组四动量的容器
        if ( nbumps != 2 ) continue;//cut条件
        
	for (unsigned ibump = 0; ibump < nbumps ; ibump++){
            PndEmcBump* bump = (PndEmcBump*)fBumpArray->At(ibump);
            Double_t bump_e = bump->energy();//获取bump能量
	    TVector3 p = bump->where();//获取bump位置
	    p.SetMag(bump_e);//利用位置设置模长，从而得到三动量
	    All_P.push_back(new TLorentzVector(p, bump_e)); //构建四动量，将指针放在容器中
        }
	TLorentzVector P_pi0 = (*All_P[0]) + (*All_P[1]);//合成两个四动量
	delete All_P[0]; delete All_P[1];//释放指针指向的空间
        h1->Fill(1000 * P_pi0.M()); //填充，乘1000是将GeV转换为MeV
    }

    h1->Draw();
    cut->Draw("SAME");
 
    TLegend * leg = new TLegend(0.69,0.72,0.88,0.85);
    leg->AddEntry(h1, "Reco", "L");
    leg->AddEntry(cut, "PDG", "L");
    leg->SetLineColor(kWhite);
    leg->Draw("SAME");

    return 0;
}

