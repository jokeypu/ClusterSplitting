#include "pi0_mass.h"

typedef struct par_n{
        std::vector< std::vector<Double_t> > R_raw, d_R_raw;
        std::vector< std::vector<Double_t> > R_new, d_R_new;
        par_n(){
                for (int i = 0; i < N_ANGLE; i++){
                        std::vector<Double_t> temp;
                        for (int j = 0; j < N_ENERGY; j++) temp.push_back(.0);
                        R_raw.push_back(temp);
                        d_R_raw.push_back(temp);
                        R_new.push_back(temp);
                        d_R_new.push_back(temp);
                }

        }
}par_n;
par_n my_par;

int Exec(int index_angle, int index_energy);

int pi0_mass_step1(){
    for (int i = 0; i < N_ANGLE; i++){
                for (int j = 0; j < N_ENERGY; j++) if ( Exec(i,j) ) return 1;
        }
        gROOT->cd();
        TFile f("doc/step1_pi0_mass.root","recreate");
        TTree tree("par","par");
        tree.Branch("R_raw", &my_par.R_raw);
        tree.Branch("d_R_raw", &my_par.d_R_raw);
        tree.Branch("R_new", &my_par.R_new);
        tree.Branch("d_R_new", &my_par.d_R_new);
        tree.Fill();
        tree.Write();
        f.Close();
        return 0;
}

int Exec(int index_angle, int index_energy){
    gROOT->SetBatch();
    int bin1 = 100, bin2 = 200;
    double xmin = 60, xmax = 200;

    TCanvas* c1 = new TCanvas("PANDA1", "c1", 1200, 900);
    gStyle->SetOptTitle(1);
    gStyle->SetStatX(0.36);
    gStyle->SetStatY(0.88);
    gStyle->SetOptStat(1);
    gStyle->SetLabelFont(42,"xyz");
    gStyle->SetLabelSize(0.05,"xyz");
    gStyle->SetLabelOffset(0.01,"xyz");
    gStyle->SetNdivisions(510,"xyz");
    gStyle->SetTitleFont(42,"xyz");
    gStyle->SetTitleColor(1,"xyz");
    gStyle->SetTitleSize(0.05,"xyz");
    gStyle->SetTitleOffset(1.0,"xyz");
    
    c1->SetLeftMargin(0.13);
    c1->SetRightMargin(-0.13);
    c1->SetTopMargin(-0.13);
    c1->SetBottomMargin(0.13);
    
    static TFile* file = new TFile("doc/pi0_mass.root", "READ");
    Float_t shower_energy = IndexToEnergy(index_energy);
    Float_t theta_min = IndexToAngle(index_angle).first;
    Float_t theta_max = IndexToAngle(index_angle).second;
    TTree *mytree = (TTree *)file->Get("pi0_test_" + NumToStr(shower_energy) +"GeV");
    if ( mytree == NULL ) return 1;
    Double_t Mass_raw, Mass_new, theta;
    mytree->SetBranchAddress("Mass_raw",&Mass_raw);
    mytree->SetBranchAddress("Mass_new",&Mass_new);
    mytree->SetBranchAddress("theta",&theta);

    TH1D* h1D1 = new TH1D("m_{#pi^{0}} Raw", Form("%.1fGeV & #theta %.2f^{o}~%.2f^{o}", shower_energy, theta_min, theta_max), bin1, xmin, xmax);
    h1D1->SetLineColor(kGray+3);
    h1D1->SetLineWidth(2);
    h1D1->GetXaxis()->SetTitle("m_{#pi0}  [MeV]");
    h1D1->GetYaxis()->SetTitle("Entries");
    h1D1->GetXaxis()->CenterTitle();
    h1D1->GetYaxis()->CenterTitle();
    
    TH1D* h1D2 = new TH1D("m_{#pi^{0}} New", Form("%.1fGeV & #theta %.2f^{o}~%.2f^{o}", shower_energy, theta_min, theta_max), bin1, xmin, xmax);
    h1D2->SetLineColor(kRed);
    h1D2->SetLineWidth(2);
    h1D2->GetXaxis()->SetTitle("m_{#pi0}  [MeV]");
    h1D2->GetYaxis()->SetTitle("Entries");
    h1D2->GetXaxis()->CenterTitle();
    h1D2->GetYaxis()->CenterTitle();
    
    Int_t maxEvtNo=(Int_t)mytree->GetEntries();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        mytree->GetEntry(ievt);
        theta *= TMath::RadToDeg();
        if ( theta > theta_max || theta < theta_min ) continue;
        h1D1->Fill(Mass_raw);
        h1D2->Fill(Mass_new);
    }

    c1->cd();

    h1D1->Draw();
    gPad->Update();
    h1D2->Draw();
    gPad->Update();
    
    TPaveStats *ps1 = (TPaveStats*)h1D1->GetListOfFunctions()->FindObject("stats");
    TPaveStats *ps2 = (TPaveStats*)h1D2->GetListOfFunctions()->FindObject("stats");
    
    ps1->SetX1NDC(0.132832);
    ps1->SetY1NDC(0.773913);
    ps1->SetX2NDC(0.333333);
    ps1->SetY2NDC(0.874783);
    
    ps2->SetX1NDC(0.132832);
    ps2->SetY1NDC(0.624348);
    ps2->SetX2NDC(0.333333);
    ps2->SetY2NDC(0.725217);

    ps1->Draw();
    ps2->Draw();    

    gPad->Update();
    h1D1->Draw("SAME");
    
    TLegend * leg = new TLegend(0.7,0.7 , 0.9, 0.8);
    leg->AddEntry(h1D1,"m_{#pi^{0}} Raw" , "L");
    leg->AddEntry(h1D2,"m_{#pi^{0}} New", "L");
    leg->Draw();
    
    double NewRange_min = 80;
    double NewRange_max = 200;
    
    int N1_max(-1), N2_max(-1);
    for (int i = 1; i <= bin1; i++){
        int N1 = h1D1->GetBinContent(i);
        int N2 = h1D2->GetBinContent(i);
    if (N1 > N1_max) N1_max = N1;
    if (N2 > N2_max) N2_max = N2;
    }
    
    Double_t set_p00 = N1_max, set_p01 = h1D1->GetMean(), set_p02 = 1.0;
    TF1 *f1=new TF1("f1","[0]*Novosibirsk(x,[1],[2],[3])",NewRange_min, NewRange_max);
    f1->SetLineColor(kBlack);
    f1->SetLineStyle(2);
    f1->SetLineWidth(2);
    f1->SetParameters(set_p00,set_p01,set_p02,0.0);
     
    Double_t set_p10 = N2_max, set_p11 = h1D2->GetMean(), set_p12 = 1.0;
    TF1 *f2=new TF1("f2","[0]*Novosibirsk(x,[1],[2],[3])",NewRange_min, NewRange_max);
    f2->SetLineColor(kRed);
    f2->SetLineStyle(2);
    f2->SetLineWidth(2);
    f2->SetParameters(set_p10,set_p11,set_p12,0.0);
     
    h1D1->Fit(f1,"R");
    h1D2->Fit(f2,"R");

    c1->cd();
    if (N1_max > N2_max){
         h1D1->Draw();
         h1D2->Draw("SAME");
    }else{
         h1D2->Draw();
         h1D1->Draw("SAME");
    }

    Double_t mean_OR = f1->GetParameter(1);
    Double_t mean_fix = f2->GetParameter(1);
    Double_t D_mean_OR = f1->GetParError(1);
    Double_t D_mean_fix = f2->GetParError(1);
    
    Double_t sigma_OR = f1->GetParameter(2);
    Double_t sigma_fix = f2->GetParameter(2);
    Double_t D_sigma_OR = f1->GetParError(2);
    Double_t D_sigma_fix = f2->GetParError(2);

    Double_t Resolution_OR = sigma_OR/mean_OR;
    Double_t Resolution_fix = sigma_fix/mean_fix;
    Double_t D_Resolution_OR = sqrt(  pow(D_sigma_OR,2) + pow(sigma_OR,2)*pow(D_mean_OR,2)/pow(mean_OR,2)  )/mean_OR;
    Double_t D_Resolution_fix = sqrt(  pow(D_sigma_fix,2) + pow(sigma_fix,2)*pow(D_mean_fix,2)/pow(mean_fix,2)  )/mean_fix;
    
    my_par.R_raw[index_angle][index_energy] = Resolution_OR;
    my_par.d_R_raw[index_angle][index_energy] = D_Resolution_OR;
    my_par.R_new[index_angle][index_energy] = Resolution_fix;
    my_par.d_R_new[index_angle][index_energy] = D_Resolution_fix;

    cout << " " << Resolution_OR << " " << D_Resolution_OR << " " << Resolution_fix << " " << D_Resolution_fix << endl;
    
    TString picture_name= "doc/step1_pi0_picture/pi0_mass_A"+ IndexToStr(index_angle) +"_E"+ NumToStr(shower_energy)  +".png";
    c1->Print(picture_name);
    
    delete c1;
    delete h1D1;
    delete h1D2;
    delete leg;
    delete f1;
    delete f2;
    return 0;
}
