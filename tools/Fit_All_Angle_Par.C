int Fit_All_Angle_Par(){
    std::string in_name = "doc/AllPar.txt";
    std::ifstream par_file;
    par_file.open(in_name,std::ios::in);
    
    TCanvas* c1=new TCanvas("PANDA1","All Angle p1",500,1000);
    TCanvas* c2=new TCanvas("PANDA2","All Angle p2",500,1000);
    TCanvas* c3=new TCanvas("PANDA3","All Angle p3",500,1000);
    TCanvas* c4=new TCanvas("PANDA4","All Angle p4",500,833);
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
    //gStyle->SetOptFit(1111);
    
    TGraph *g11 = new TGraph();
    g11->SetMarkerStyle(20);
    g11->SetMarkerColorAlpha(kRed-3, 1);
    g11->GetXaxis()->SetTitle("Energy");
    g11->GetYaxis()->SetTitle("p1");
    g11->GetXaxis()->CenterTitle();
    g11->GetYaxis()->CenterTitle();
    
    TGraph *g12 = new TGraph();
    g12->SetMarkerStyle(22);
    g12->SetMarkerColorAlpha(kGreen+1, 1);
    g12->GetXaxis()->SetTitle("Energy");
    g12->GetYaxis()->SetTitle("p2");
    g12->GetXaxis()->CenterTitle();
    g12->GetYaxis()->CenterTitle();
    
    TGraph *g13 = new TGraph();
    g13->SetMarkerStyle(33);
    g13->SetMarkerColorAlpha(kAzure+3, 1);
    g13->GetXaxis()->SetTitle("Energy");
    g13->GetYaxis()->SetTitle("p3");
    g13->GetXaxis()->CenterTitle();
    g13->GetYaxis()->CenterTitle();
    
    TGraph *g21 = new TGraph();
    g21->SetMarkerStyle(20);
    g21->SetMarkerColorAlpha(kRed-3, 1);
    g21->GetXaxis()->SetTitle("Energy");
    g21->GetYaxis()->SetTitle("p1");
    g21->GetXaxis()->CenterTitle();
    g21->GetYaxis()->CenterTitle();
    
    TGraph *g22 = new TGraph();
    g22->SetMarkerStyle(22);
    g22->SetMarkerColorAlpha(kGreen+1, 1);
    g22->GetXaxis()->SetTitle("Energy");
    g22->GetYaxis()->SetTitle("p2");
    g22->GetXaxis()->CenterTitle();
    g22->GetYaxis()->CenterTitle();
    
    TGraph *g23 = new TGraph();
    g23->SetMarkerStyle(33);
    g23->SetMarkerColorAlpha(kAzure+3, 1);
    g23->GetXaxis()->SetTitle("Energy");
    g23->GetYaxis()->SetTitle("p3");
    g23->GetXaxis()->CenterTitle();
    g23->GetYaxis()->CenterTitle();
    
    TGraph *g31 = new TGraph();
    g31->SetMarkerStyle(20);
    g31->SetMarkerColorAlpha(kRed-3, 1);
    g31->GetXaxis()->SetTitle("Energy");
    g31->GetYaxis()->SetTitle("p1");
    g31->GetXaxis()->CenterTitle();
    g31->GetYaxis()->CenterTitle();
    
    TGraph *g32 = new TGraph();
    g32->SetMarkerStyle(22);
    g32->SetMarkerColorAlpha(kGreen+1, 1);
    g32->GetXaxis()->SetTitle("Energy");
    g32->GetYaxis()->SetTitle("p2");
    g32->GetXaxis()->CenterTitle();
    g32->GetYaxis()->CenterTitle();
    
    TGraph *g33 = new TGraph();
    g33->SetMarkerStyle(33);
    g33->SetMarkerColorAlpha(kAzure+3, 1);
    g33->GetXaxis()->SetTitle("Energy");
    g33->GetYaxis()->SetTitle("p3");
    g33->GetXaxis()->CenterTitle();
    g33->GetYaxis()->CenterTitle();
    
    TGraph *g41 = new TGraph();
    g41->SetMarkerStyle(20);
    g41->SetMarkerColorAlpha(kRed-3, 1);
    g41->GetXaxis()->SetTitle("Energy");
    g41->GetYaxis()->SetTitle("p1");
    g41->GetXaxis()->CenterTitle();
    g41->GetYaxis()->CenterTitle();
    
    TGraph *g42 = new TGraph();
    g42->SetMarkerStyle(22);
    g42->SetMarkerColorAlpha(kGreen+1, 1);
    g42->GetXaxis()->SetTitle("Energy");
    g42->GetYaxis()->SetTitle("p2");
    g42->GetXaxis()->CenterTitle();
    g42->GetYaxis()->CenterTitle();
    
    string str;
    Int_t N = 0;
    while (std::getline(par_file, str)) {
        std::stringstream strStream(str);
        float angle, p11, p12, p13, p21, p22, p23, p31, p32, p33, p41, p42;
        strStream >> angle >> p11 >> p12 >> p13 >> p21 >> p22 >> p23 >> p31 >> p32 >> p33 >> p41 >> p42;
        g11->SetPoint(N,10*angle,p11);
        g12->SetPoint(N,10*angle,p12);
        g13->SetPoint(N,10*angle,p13);
        
        g21->SetPoint(N,10*angle,p21);
        g22->SetPoint(N,10*angle,p22);
        g23->SetPoint(N,10*angle,p23);
        
        g31->SetPoint(N,10*angle,p31);
        g32->SetPoint(N,10*angle,p32);
        g33->SetPoint(N,10*angle,p33);
        
        g41->SetPoint(N,10*angle,p41);
        g42->SetPoint(N,10*angle,p42);
        
        N++;
    }
    par_file.close();

    TF1* f11=new TF1("f11","[0]",30,130);
    f11->SetLineColor(kRed-7);
    
    TF1* f12=new TF1("f12","[0]",30,130);
    f12->SetLineColor(kRed-7);
    
    TF1* f13=new TF1("f13","[0]*(x-[1])*(x-[1])+[2]",30,130);
    f13->SetLineColor(kRed-7);
    f13->SetParameters(0.04,80,0.004);
    
    TF1* f21=new TF1("f21","[0]",30,130);
    f21->SetLineColor(kRed-7);
    
    TF1* f22=new TF1("f22","[0]",30,130);
    f22->SetLineColor(kRed-7);
    
    //TF1* f23=new TF1("f23","[0]*(x-[1])*(x-[1])+[2]",30,130);
    TF1* f23=new TF1("f23","[0]*(x-[1])*(x-[1])+[2]",30,130);
    f23->SetLineColor(kRed-7);
    //f23->SetParameters(0.04,80,0.004);
    
    TF1* f31=new TF1("f31","[0]",30,130);
    f31->SetLineColor(kRed-7);
    
    TF1* f32=new TF1("f32","[0]",30,130);
    f32->SetLineColor(kRed-7);
    
    TF1* f33=new TF1("f33","[0]*(x-[1])*(x-[1])+[2]",30,130);
    f33->SetLineColor(kRed-7);
    f33->SetParameters(0.04,80,0.004);
    
    TF1* f41=new TF1("f41","[0]",30,130);
    f41->SetLineColor(kRed-7);
    
    TF1* f42=new TF1("f42","[0]",30,130);
    f42->SetLineColor(kRed-7);
    
    g11->Fit(f11,"R");
    g12->Fit(f12,"R");
    g13->Fit(f13,"R");
    g21->Fit(f21,"R");
    g22->Fit(f22,"R");
    g23->Fit(f23,"R");
    g31->Fit(f31,"R");
    g32->Fit(f32,"R");
    g33->Fit(f33,"R");
    g41->Fit(f41,"R");
    g42->Fit(f42,"R");
    
    c1->Divide(1, 3);
    c1->cd(1);
    //c1->cd(1)->SetGridx();
    g11->Draw("AP.");
    c1->cd(2);
    //c1->cd(2)->SetGridx();
    g12->Draw("AP.");
    c1->cd(3);
    //c1->cd(3)->SetGridx();
    g13->Draw("AP.");
    
    c2->Divide(1, 3);
    c2->cd(1);
    //c1->cd(1)->SetGridx();
    g21->Draw("AP.");
    c2->cd(2);
    //c1->cd(2)->SetGridx();
    g22->Draw("AP.");
    c2->cd(3);
    //c1->cd(3)->SetGridx();
    g23->Draw("AP.");
    
    c3->Divide(1, 3);
    c3->cd(1);
    //c1->cd(1)->SetGridx();
    g31->Draw("AP.");
    c3->cd(2);
    //c1->cd(2)->SetGridx();
    g32->Draw("AP.");
    c3->cd(3);
    //c1->cd(3)->SetGridx();
    g33->Draw("AP.");
    
    c4->Divide(1, 2);
    c4->cd(1);
    //c1->cd(1)->SetGridx();
    g41->Draw("AP.");
    c4->cd(2);
    //c1->cd(2)->SetGridx();
    g42->Draw("AP.");
    
    cout << "Double_t mp1[3] = {" << f11->GetParameter(0) << ", " << f12->GetParameter(0) << ", "
         << f13->GetParameter(0) << "*(ShowerAngle-" << f13->GetParameter(1) << ")" << "*(ShowerAngle-" << f13->GetParameter(1) << ")+" << f13->GetParameter(2) << "};" << endl;
    cout << "Double_t mp2[3] = {" << f21->GetParameter(0) << ", " << f22->GetParameter(0) << ", "
    << f23->GetParameter(0) << "*(ShowerAngle-" << f23->GetParameter(1) << ")" << "*(ShowerAngle-" << f23->GetParameter(1) << ")+" << f23->GetParameter(2) << "};" << endl;
    cout << "Double_t mp3[3] = {" << f31->GetParameter(0) << ", " << f32->GetParameter(0) << ", "
    << f33->GetParameter(0) << "*(ShowerAngle-" << f33->GetParameter(1) << ")" << "*(ShowerAngle-" << f33->GetParameter(1) << ")+" << f33->GetParameter(2) << "};" << endl;
    cout << "Double_t mp4[2] = {" << f41->GetParameter(0) << ", " << f42->GetParameter(0) << "};" << endl;
    
    /*c1->Print("doc/All_Angle_FitPar_p1.png");
    c2->Print("doc/All_Angle_FitPar_p2.png");
    c3->Print("doc/All_Angle_FitPar_p3.png");
    c4->Print("doc/All_Angle_FitPar_p4.png");*/
    return 0;
}
