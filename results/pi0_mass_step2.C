#include "pi0_mass.h"
std::vector< std::vector<Double_t> > *R_raw(NULL), *d_R_raw(NULL);
std::vector< std::vector<Double_t> > *R_new(NULL), *d_R_new(NULL);
int Exec(int index_angle);

int pi0_mass_step2(){
    TFile* file = new TFile("doc/step1_pi0_mass.root", "READ");
    TTree *mytree = (TTree *)file->Get("par");
    mytree->SetBranchAddress("R_raw",&R_raw);
    mytree->SetBranchAddress("d_R_raw",&d_R_raw);
    mytree->SetBranchAddress("R_new",&R_new);
    mytree->SetBranchAddress("d_R_new",&d_R_new);
    mytree->GetEntry(0);
    for (int index_angle = 0; index_angle < N_ANGLE; index_angle++) if ( Exec(index_angle) ) return 1;
    file->Close();
    delete file;
    return 0;
}

int Exec(int index_angle){
    gROOT->SetBatch();
    Float_t theta_min = IndexToAngle(index_angle).first;
    Float_t theta_max = IndexToAngle(index_angle).second;

    TCanvas* c1=new TCanvas("PANDA1",Form("#theta %.2f^{o}~%.2f^{o}", theta_min, theta_max),1000,700);
    gStyle->SetOptTitle(1);
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

    c1->SetLeftMargin(0.13);
    c1->SetRightMargin(-0.13);
    c1->SetTopMargin(-0.13);
    c1->SetBottomMargin(0.13);
    
    TGraph *g1 = new TGraph();
    g1->SetMarkerStyle(20);
    g1->SetMarkerColorAlpha(kRed-3, 1);
    g1->GetXaxis()->SetTitle("Energy");
    g1->GetYaxis()->SetTitle("Resolution");
    g1->GetXaxis()->CenterTitle();
    g1->GetYaxis()->CenterTitle();
    
    TGraph *g2 = new TGraph();
    g2->SetMarkerStyle(22);
    g2->SetMarkerColorAlpha(kBlack+1, 1);
    g2->GetXaxis()->SetTitle("Energy");
    g2->GetYaxis()->SetTitle("Resolution");
    g2->GetXaxis()->CenterTitle();
    g2->GetYaxis()->CenterTitle();
    
    double R1_x[N_ENERGY], R1_y[N_ENERGY], R1_Ex[N_ENERGY], R1_Ey[N_ENERGY], R1_Ey_m[N_ENERGY], R1_Ey_p[N_ENERGY];
    double R2_x[N_ENERGY], R2_y[N_ENERGY], R2_Ex[N_ENERGY], R2_Ey[N_ENERGY], R2_Ey_m[N_ENERGY], R2_Ey_p[N_ENERGY];
    for (int index_energy = 0; index_energy < N_ENERGY; index_energy++){
        Float_t pi0_mom = IndexToEnergy(index_energy);
        R1_x[index_energy] = pi0_mom;
        R1_y[index_energy] = 100*(*R_raw)[index_angle][index_energy];
        R1_Ex[index_energy] = 0.0;
        R1_Ey[index_energy] = 100*(*d_R_raw)[index_angle][index_energy];
        R2_x[index_energy] = pi0_mom;
        R2_y[index_energy] = 100*(*R_new)[index_angle][index_energy];
        R2_Ex[index_energy] = 0.0;
        R2_Ey[index_energy] = 100*(*d_R_new)[index_angle][index_energy];
    }
    
    TGraphErrors *gr1 = new TGraphErrors(N_ENERGY,R1_x,R1_y,R1_Ex,R1_Ey);
    gr1->SetMarkerStyle(22);
    gr1->SetMarkerColorAlpha(kBlack, 1);
    gr1->SetTitle(Form("#theta %.2f^{o}~%.2f^{o}", theta_min, theta_max));
    gr1->GetXaxis()->SetTitle("P_{#pi^{0}}   [GeV/c]");
    gr1->GetYaxis()->SetTitle("#sigma(m_{#pi^{0}})/m_{#pi^{0}}   [%]");
    gr1->GetYaxis()->SetTitleOffset(1.0);
    gr1->GetXaxis()->CenterTitle();
    gr1->GetYaxis()->CenterTitle();
    
    TGraphErrors *gr2 = new TGraphErrors(N_ENERGY,R2_x,R2_y,R2_Ex,R2_Ey);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerColorAlpha(kRed, 1);
    gr2->SetTitle(Form("#theta %.2f^{o}~%.2f^{o}", theta_min, theta_max));
    gr2->GetXaxis()->SetTitle("P_{#pi^{0}}   [GeV/c]");
    gr2->GetYaxis()->SetTitle("#sigma(m_{#pi^{0}})/m_{#pi^{0}}   [%]");
    gr2->GetYaxis()->SetTitleOffset(1.0);
    gr2->GetXaxis()->CenterTitle();
    gr2->GetYaxis()->CenterTitle();

    gr1->Draw("APL");
    gr2->Draw("PLsame");
    
    TLegend * leg1 = new TLegend(0.254108,0.76,0.424649,0.89037);
    leg1->AddEntry(gr1, "Raw algorithm", "EP");
    leg1->AddEntry(gr2, "New algorithm", "EP");
    leg1->SetBorderSize(1);
    leg1->Draw("SAME");

    TString picture_name= "doc/step2_pi0_picture/pi0_mass_A" + IndexToStr(index_angle) + ".png";
    c1->Print(picture_name);

    delete c1;
    delete g1;
    delete g2;
    delete gr1;
    delete gr2;
    delete leg1;
    return 0;
}
