const Double_t X0 = 0.89;
const Double_t RM = 2.00;
Double_t FABC(Double_t t,Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4){
    Double_t xi = t - p2*t*exp(-pow(X0*t/p3/RM,p4));
    return p0*exp(-p1*xi*X0/RM);
}

int Fit_DigiEnergy_cp(std::string dir_name, const char title[30], Int_t NO_Angle, Double_t Energy){
    //title[20] = "doc/"+ dir_name +"_R.txt"
    ostringstream out1,out2;
    out1 << NO_Angle;
    out2 << fixed << setprecision(1) << Energy;
    string str_NO_Angle = out1.str(), str_Energy = out2.str();
    std::string out_name = "doc/A"+str_NO_Angle+"_par_cp.txt";
    std::ofstream par_file;
    par_file.open(out_name,std::ios::app);
    
    TCanvas* c2=new TCanvas("PANDA2","test2",800,600);
    TCanvas* c1=new TCanvas("PANDA1","test1",800,600);
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
    gStyle->SetTitleOffset(1.2,"xyz");
    //gStyle->SetPalette(1);
    
    std::string file_name(title);
    std::ifstream in_file;
    in_file.open(file_name,std::ios::in);
    
    TGraph *g = new TGraph();
    g->SetMarkerStyle(20);
    g->SetMarkerColor(kYellow);
    g->GetYaxis()->SetTitle("E_{truth}   [GeV]");
    g->GetXaxis()->SetTitle("t   [X_{0}]");
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();
    
    string str;
    Int_t N = 0;
    Double_t distance_cut = 3.5/X0;
    Int_t binx = 200, biny = 200;
    
    TH2D* h = new TH2D("Hist", "h", binx, 0, distance_cut, biny, 0, Energy);
    h->SetMarkerStyle(7);
    h->SetMarkerColorAlpha(kAzure+3, 1);
    h->GetYaxis()->SetTitle("E_{truth}   [GeV]");
    h->GetXaxis()->SetTitle("t   [X_{0}]");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();
    
    while (std::getline(in_file, str)) {
        std::stringstream strStream(str);
        float distance, angle, energy;
        strStream >> distance >> angle >> energy;
        if (distance > distance_cut) continue;
        h->Fill(distance/X0,energy);
        N++;
    }
    
    int cunt = 0;
    int step = 7;
    for (int i = 1; i < binx+1; i+=step){
        Double_t mean = h->ProfileY("px",i,i+step-1)->GetMean();
        Double_t wx = distance_cut/binx;
        Double_t nx =( (i+(step-1)/2)*wx - wx/2  );
        g->SetPoint(cunt, nx, mean);
        cunt++;
    }
    
    for (int i = 1; i < 201; i++){
        for (int j = 1; j < 201; j++){
            if (N < 30000) {if (h->GetBinContent(i,j)<2) h->SetBinContent(i,j,0);}
            else if (h->GetBinContent(i,j)<2) h->SetBinContent(i,j,0);
        }
    }
    
    TF1* f=new TF1("f1","(FABC(x,[0],[1],[2],[3],[4]))",0,distance_cut);
    f->SetParameters(0.8*Energy, 2.5, 0.9, 0.7, 3);
    f->SetLineWidth(3);
    f->SetLineColor(kRed);

    c1->cd();
    h->Draw("PCOLZ");
    g->Draw("Psame");
    g->Fit(f,"R");
    par_file << str_Energy << " " << f->GetParameter(0) << " " << f->GetParameter(1) << " " << f->GetParameter(2) << " " << f->GetParameter(3) << " " << f->GetParameter(4) << endl;
    
    TString picture_name= "doc/A"+str_NO_Angle+"_FitPicture_cp/A"+str_NO_Angle+"_E"+str_Energy+"_FitPar_cp.png";
    c1->Print(picture_name);
    
    TGraph *g_Error = new TGraph();
    g_Error->SetMarkerStyle(7);
    g_Error->SetMarkerColorAlpha(kAzure+3, 0.5);
    g_Error->GetYaxis()->SetTitle("E_{func}-E_{truth}   [GeV]");
    g_Error->GetXaxis()->SetTitle("t   [X_{0}]");
    g_Error->GetXaxis()->CenterTitle();
    g_Error->GetYaxis()->CenterTitle();
    
    TH2D* h_Error = new TH2D("Hist_Error","h_Error",200,0,distance_cut,200,-Energy,Energy);
    h_Error->SetMarkerStyle(7);
    h_Error->SetMarkerColorAlpha(kAzure+3, 0.5);
    h_Error->GetYaxis()->SetTitle("E_{func}-E_{truth}   [GeV]");
    h_Error->GetXaxis()->SetTitle("t   [X_{0}]");
    h_Error->GetXaxis()->CenterTitle();
    h_Error->GetYaxis()->CenterTitle();
    h_Error->GetZaxis()->CenterTitle();
    
    in_file.clear();
    in_file.seekg(0, ios::beg);
    N = 0;
    while (std::getline(in_file, str)) {
        std::stringstream strStream(str);
        float distance, angle, energy;
        strStream >> distance >> angle >> energy;
        if (distance > distance_cut) continue;
        h_Error->Fill(distance/X0,(FABC(distance/X0,f->GetParameter(0),f->GetParameter(1),f->GetParameter(2),f->GetParameter(3),f->GetParameter(4))-energy));
        N++;
    }
    c2->cd();
    h_Error->Draw("PCOLZ");
    TString picture_name_error= "doc/A"+str_NO_Angle+"_FitPicture_cp/Error_A"+str_NO_Angle+"_E"+str_Energy+"_FitPar_cp.png";
    c2->Print(picture_name_error);
    
    in_file.close();
    par_file.close();
    return 0;
}
