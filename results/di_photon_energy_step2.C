#include "di_photon_energy.h"
std::vector<Double_t> *m_raw(NULL), *d_m_raw(NULL), *R_raw(NULL), *d_Rm_raw(NULL), *d_Rp_raw(NULL);
std::vector<Double_t> *m_new(NULL), *d_m_new(NULL), *R_new(NULL), *d_Rm_new(NULL), *d_Rp_new(NULL);
int my_index_angle;
int Exec(int index_angle);

int di_photon_energy_step2(){
    TFile* file = new TFile("doc/step1_di-photon_energy.root", "READ");
    TTree *mytree = (TTree *)file->Get("par");
    mytree->SetBranchAddress("m_raw",&m_raw);
    mytree->SetBranchAddress("d_m_raw",&d_m_raw);
    mytree->SetBranchAddress("R_raw",&R_raw);
    mytree->SetBranchAddress("d_Rm_raw",&d_Rm_raw);
    mytree->SetBranchAddress("d_Rp_raw",&d_Rp_raw);
    mytree->SetBranchAddress("m_new",&m_new);
    mytree->SetBranchAddress("d_m_new",&d_m_new);
    mytree->SetBranchAddress("R_new",&R_new);
    mytree->SetBranchAddress("d_Rm_new",&d_Rm_new);
    mytree->SetBranchAddress("d_Rp_new",&d_Rp_new);
    mytree->SetBranchAddress("theta",&my_index_angle);
    mytree->GetEntry(0);
    if ( Exec(my_index_angle) ) return 1;
    file->Close();
    delete file;
    return 0;
}

int Exec(int index_angle){
    gROOT->SetBatch();
    Float_t theta_min = IndexToAngle(index_angle).first;
    Float_t theta_max = IndexToAngle(index_angle).second;

    TCanvas* c1=new TCanvas("PANDA1",Form("#theta %.2f^{o}", (theta_min+theta_max)/2.0),1000,700);
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
        Float_t energy = IndexToEnergy(index_energy);
        R1_x[index_energy] = (*m_raw)[index_energy];
        R1_y[index_energy] = 100*(*R_raw)[index_energy];
        R1_Ex[index_energy] = (*d_m_raw)[index_energy];
        R1_Ey[index_energy] = 100*((*d_Rp_raw)[index_energy] > (*d_Rm_raw)[index_energy] ? (*d_Rp_raw)[index_energy] : (*d_Rm_raw)[index_energy]);
        R2_x[index_energy] = (*m_new)[index_energy];
        R2_y[index_energy] = 100*(*R_new)[index_energy];
        R2_Ex[index_energy] = (*d_m_new)[index_energy];
        R2_Ey[index_energy] = 100*((*d_Rp_new)[index_energy] > (*d_Rm_new)[index_energy] ? (*d_Rp_new)[index_energy] : (*d_Rm_new)[index_energy]);
    }
    
    TGraphErrors *gr1 = new TGraphErrors(N_ENERGY,R1_x,R1_y,R1_Ex,R1_Ey);
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColorAlpha(kBlack, 1);
    gr1->GetXaxis()->SetTitle("E_{#gamma}   [GeV/c^{2}]");
    gr1->GetYaxis()->SetTitle("#sigma(E_{#gamma})/E_{#gamma}   [%]");
    gr1->GetYaxis()->SetTitleOffset(1.0);
    gr1->GetXaxis()->CenterTitle();
    gr1->GetYaxis()->CenterTitle();
    
    TGraphErrors *gr2 = new TGraphErrors(N_ENERGY,R2_x,R2_y,R2_Ex,R2_Ey);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerColorAlpha(kRed, 1);
    gr2->GetXaxis()->SetTitle("E_{#gamma}   [GeV/c^{2}]");
    gr2->GetYaxis()->SetTitle("#sigma(E_{#gamma})/E_{#gamma}   [%]");
    gr2->GetYaxis()->SetTitleOffset(1.0);
    gr2->GetXaxis()->CenterTitle();
    gr2->GetYaxis()->CenterTitle();

    gr1->GetYaxis()->SetRangeUser(4,12);
    gr2->GetYaxis()->SetRangeUser(4,12);
    
    gr1->Draw("AP");
    gr2->Draw("Psame");
    
    TF1 *f1 = new TF1("f1","[0]/sqrt(x) + [1]",0,6.5);
    f1->SetLineColor(kBlack);
    f1->SetLineWidth(2);
    f1->SetLineStyle(2);
    f1->SetParName(0, "#sigma_{0}");
    f1->SetParName(1, "#sigma_{1}");
    
    TF1 *f2 = new TF1("f2","[0]/sqrt(x) + [1]",0,6.5);
    f2->SetLineColor(kRed);
    f2->SetLineWidth(2);
    f2->SetLineStyle(2);
    f2->SetParName(0, "#sigma_{0}");
    f2->SetParName(1, "#sigma_{1}");
    
    gr1->Fit(f1,"R");
    gr2->Fit(f2,"R");
    
    ostringstream os1, os2, os3, os4;
    os1 << f1->GetParameter(0);
    os2 << f1->GetParameter(1);
    os3 << f2->GetParameter(0);
    os4 << f2->GetParameter(1);
    string par1_str = os1.str(), par2_str = os2.str(), par3_str = os3.str(), par4_str = os4.str();
    string sss1 = par1_str+"/#sqrt{E} + "+par2_str;
    string sss2 = par3_str+"/#sqrt{E} + "+par4_str;
    cout << sss1 << endl;
    cout << sss2 << endl;
    
    float N_per = 100.0;
    TPaveText *pt = new TPaveText(0.501002, 0.48, 0.877756, 0.736296,"NDC");
    TText *t1 = pt->AddText(Form("Raw:  #frac{#sigma(E_{#gamma})}{E_{#gamma}}  =  #frac{(%g #pm %g) %%}{#sqrt{E_{#gamma}}} + (%g #pm %g) %%",round(N_per*(float)f1->GetParameter(0))/N_per,round(N_per*(float)f1->GetParError(0))/N_per,round(N_per*(float)f1->GetParameter(1))/N_per,round(N_per*(float)f1->GetParError(1))/N_per));
    pt->AddLine(.0,.5,1.,.5);
    TText *t2 = pt->AddText(Form("New:  #frac{#sigma(E_{#gamma})}{E_{#gamma}}  =  #frac{(%g #pm %g) %%}{#sqrt{E_{#gamma}}} + (%g #pm %g) %%",round(N_per*(float)f2->GetParameter(0))/N_per, round(N_per*(float)f2->GetParError(0))/N_per,round(N_per*(float)f2->GetParameter(1))/N_per,round(N_per*(float)f2->GetParError(1))/N_per));
    
    t1->SetTextColor(kBlack);
    t2->SetTextColor(kRed);
    t1->SetTextSize(0.028);
    t2->SetTextSize(0.028);
    pt->Draw();

    TLegend * leg1 = new TLegend(0.554108,0.76,0.824649,0.89037);
    leg1->AddEntry(gr1, "Raw algorithm", "EP");
    leg1->AddEntry(gr2, "New algorithm", "EP");
    leg1->SetBorderSize(1);
    leg1->Draw("SAME");

    TString picture_name= "doc/step2_di-photon_picture/di-photon_energy_A" + IndexToStr(index_angle) + ".png";
    c1->Print(picture_name);
    delete c1;
    delete g1;
    delete g2;
    delete gr1;
    delete gr2;
    delete f1;
    delete f2;
    delete pt;
    delete leg1;
    return 0;
}
