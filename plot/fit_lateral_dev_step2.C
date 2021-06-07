#include "fit_lateral_dev.h"
std::vector< std::vector<Double_t> > *p1(NULL), *p2(NULL), *p3(NULL), *p4(NULL);
std::vector< std::vector<Double_t> > n_p1, n_p2, n_p3, n_p4;
void Init();
int Exec(int index_angle);

int fit_lateral_dev_step2(){
    Init();
    TFile* file = new TFile("doc/step1_fit_par.root", "READ");
    TTree *mytree = (TTree *)file->Get("par");
    mytree->SetBranchAddress("p1",&p1);
    mytree->SetBranchAddress("p2",&p2);
    mytree->SetBranchAddress("p3",&p3);
    mytree->SetBranchAddress("p4",&p4);
    mytree->GetEntry(0);
    for (int index_angle = 0; index_angle < N_ANGLE; index_angle++) if ( Exec(index_angle) ) return 1;
    file->Close();
    delete file;
    gROOT->cd();
    TFile f("doc/step2_fit_par.root","recreate");
    TTree tree("step2_par","step2_par");
    tree.Branch("n_p1", &n_p1);
    tree.Branch("n_p2", &n_p2);
    tree.Branch("n_p3", &n_p3);
    tree.Branch("n_p4", &n_p4);
    tree.Fill();
    tree.Write();
    f.Close();
    return 0;
}

int Exec(int index_angle){
    gROOT->SetBatch();
    TCanvas* c1 = new TCanvas("c1","Energy dependecy",800,600);
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
    gStyle->SetTitleOffset(1.2,"xyz");
    //gStyle->SetPalette(1);

    c1->SetLeftMargin(0.13);
    c1->SetRightMargin(0.13);
    c1->SetTopMargin(-0.13);
    c1->SetBottomMargin(0.13);

    Float_t theta_min = IndexToAngle(index_angle).first;
    Float_t theta_max = IndexToAngle(index_angle).second;

    TGraph *g1 = new TGraph();
    g1->SetMarkerStyle(21);
    g1->SetMarkerColorAlpha(kRed-3, 1);
    g1->GetXaxis()->SetTitle("Energy");
    g1->GetYaxis()->SetTitle("p1");
    g1->GetXaxis()->CenterTitle();
    g1->GetYaxis()->CenterTitle();
    
    TGraph *g2 = new TGraph();
    g2->SetMarkerStyle(22);
    g2->SetMarkerColorAlpha(kGreen+1, 1);
    g2->GetXaxis()->SetTitle("Energy");
    g2->GetYaxis()->SetTitle("p2");
    g2->GetXaxis()->CenterTitle();
    g2->GetYaxis()->CenterTitle();
    
    TGraph *g3 = new TGraph();
    g3->SetMarkerStyle(33);
    g3->SetMarkerColorAlpha(kAzure+3, 1);
    g3->GetXaxis()->SetTitle("Energy");
    g3->GetYaxis()->SetTitle("p3");
    g3->GetXaxis()->CenterTitle();
    g3->GetYaxis()->CenterTitle();
    
    TGraph *g4 = new TGraph();
    g4->SetMarkerStyle(34);
    g4->SetMarkerColorAlpha(kBlue+1, 1);
    g4->GetXaxis()->SetTitle("Energy");
    g4->GetYaxis()->SetTitle("p4");
    g4->GetXaxis()->CenterTitle();
    g4->GetYaxis()->CenterTitle();
    
    Int_t N = 0;
    Float_t Max_Energy = .0;
    for (int index_energy = 0; index_energy < N_ENERGY; index_energy++){
        Float_t energy = IndexToEnergy(index_energy);
        g1->SetPoint(N,energy,(*p1)[index_angle][index_energy]);
        g2->SetPoint(N,energy,(*p2)[index_angle][index_energy]);
        g3->SetPoint(N,energy,(*p3)[index_angle][index_energy]);
        g4->SetPoint(N,energy,(*p4)[index_angle][index_energy]);
        if (energy>Max_Energy) Max_Energy = energy;
        N++;
    }

    TF1* f1=new TF1("f1","[0]*exp(-[1]*x)+[2]",0,Max_Energy);
    f1->SetLineColor(kRed-7);
    f1->SetParameters(-0.35, 2.4, 2.7);
    f1->SetParLimits(0,-0.4,-0.3);
    f1->SetParLimits(1,2.0, 5.0);
    f1->SetParLimits(2,2.0,3.0);

    TF1* f2=new TF1("f2","[0]*exp(-[1]*x)+[2]",0,Max_Energy);
    f2->SetLineColor(kRed-7);
    f2->SetParameters(-0.3,3.0,0.9);
    f2->SetParLimits(0,-0.4,-0.1);
    f2->SetParLimits(1,1.0,5.0);
    f2->SetParLimits(2,0.5,1.0);
    
    TF1* f3=new TF1("f3","[0]*exp(-[1]*x)+[2]",0,Max_Energy);
    f3->SetLineColor(kRed-7);
    f3->SetParameters(0.2,6.0,0.77);
    f3->SetParLimits(0,0.1,0.2);
    f3->SetParLimits(1,1.0,8.0);
    f3->SetParLimits(2,0.5,1.0);
    
    TF1* f4=new TF1("f4","[0]*exp(-[1]*x)+[2]",0,Max_Energy);
    f4->SetLineColor(kRed-7);
    f4->SetParameters(-2.2,0.9,5.27);
    f4->SetParLimits(0,-3.0,-1.0);
    f4->SetParLimits(1,0.5,1.2);
    f4->SetParLimits(2,2.5,7.0);
    
    g1->Fit(f1,"R");
    g2->Fit(f2,"R");
    g3->Fit(f3,"R");
    g4->Fit(f4,"R");

    g1->GetYaxis()->SetRangeUser(2.0,3.3);
    g2->GetYaxis()->SetRangeUser(0.5,1.1);
    g3->GetYaxis()->SetRangeUser(0.5,1.1);
    g4->GetYaxis()->SetRangeUser(2.5,7.5);
    
    c1->Divide(2, 2);
    c1->cd(1); g1->Draw("AP.");
    c1->cd(2); g2->Draw("AP.");
    c1->cd(3); g3->Draw("AP.");
    c1->cd(4); g4->Draw("AP.");
    
    n_p1[index_angle][0] = f1->GetParameter(0);
    n_p1[index_angle][1] = f1->GetParameter(1);
    n_p1[index_angle][2] = f1->GetParameter(2);

    n_p2[index_angle][0] = f2->GetParameter(0);
    n_p2[index_angle][1] = f2->GetParameter(1);
    n_p2[index_angle][2] = f2->GetParameter(2);

    n_p3[index_angle][0] = f3->GetParameter(0);
    n_p3[index_angle][1] = f3->GetParameter(1);
    n_p3[index_angle][2] = f3->GetParameter(2);

    n_p4[index_angle][0] = f4->GetParameter(0);
    n_p4[index_angle][1] = f4->GetParameter(1);
    n_p4[index_angle][2] = f4->GetParameter(2);
    
    TString picture_name= "doc/step2_picture/energy_dependecy_A" +  IndexToStr(index_angle) + ".png";
    c1->Print(picture_name);
    delete c1;
    delete g1;
    delete g2;
    delete g3;
    delete g4;
    delete f1;
    delete f2;
    delete f3;
    delete f4;
    return 0;
}
void Init(){
    for (int i = 0; i < N_ANGLE; i++){
        std::vector<Double_t> temp;
        for (int j = 0; j < 3; j++) temp.push_back(.0);
        n_p1.push_back(temp);
        n_p2.push_back(temp);
        n_p3.push_back(temp);
        n_p4.push_back(temp);
    }
}
