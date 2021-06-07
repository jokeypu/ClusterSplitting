#include "fit_lateral_dev.h"
std::vector< std::vector<Double_t> > *n_p1(NULL), *n_p2(NULL), *n_p3(NULL), *n_p4(NULL);
int Exec();

int fit_lateral_dev_step3(){
    TFile* file = new TFile("doc/step2_fit_par.root", "READ");
    TTree *mytree = (TTree *)file->Get("step2_par");
    mytree->SetBranchAddress("n_p1",&n_p1);
    mytree->SetBranchAddress("n_p2",&n_p2);
    mytree->SetBranchAddress("n_p3",&n_p3);
    mytree->SetBranchAddress("n_p4",&n_p4);
    mytree->GetEntry(0);
    if ( Exec() ) return 1;
    file->Close();
    return 0;
}

int Exec(){
    gROOT->SetBatch();
    double ty0 = 1200, tx0 = 800;
    TCanvas* c1 = new TCanvas("PANDA0","All Angle p1",tx0,ty0);
    TCanvas* c2 = new TCanvas("PANDA1","All Angle p2",tx0,ty0);
    TCanvas* c3 = new TCanvas("PANDA3","All Angle p3",tx0,ty0);
    TCanvas* c4 = new TCanvas("PANDA2","All Angle p4",tx0,ty0);
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

    c1->SetLeftMargin(0.13);
    c1->SetRightMargin(-0.13);
    c1->SetTopMargin(-0.13);
    c1->SetBottomMargin(0.13);
    
    c2->SetLeftMargin(0.13);
    c2->SetRightMargin(-0.13);
    c2->SetTopMargin(-0.13);
    c2->SetBottomMargin(0.13);
    
    c3->SetLeftMargin(0.13);
    c3->SetRightMargin(-0.13);
    c3->SetTopMargin(-0.13);
    c3->SetBottomMargin(0.13);
    
    c4->SetLeftMargin(0.13);
    c4->SetRightMargin(-0.13);
    c4->SetTopMargin(-0.13);
    c4->SetBottomMargin(0.13);
    
    TGraph *g11 = new TGraph();
    g11->SetMarkerStyle(22);
    g11->SetMarkerColorAlpha(kGreen+1, 1);
    g11->GetXaxis()->SetTitle("#theta(deg)");
    g11->GetYaxis()->SetTitle("A");
    g11->GetXaxis()->CenterTitle();
    g11->GetYaxis()->CenterTitle();
    
    TGraph *g12 = new TGraph();
    g12->SetMarkerStyle(22);
    g12->SetMarkerColorAlpha(kGreen+1, 1);
    g12->GetXaxis()->SetTitle("#theta(deg)");
    g12->GetYaxis()->SetTitle("#kappa");
    g12->GetXaxis()->CenterTitle();
    g12->GetYaxis()->CenterTitle();
    
    TGraph *g13 = new TGraph();
    g13->SetMarkerStyle(22);
    g13->SetMarkerColorAlpha(kRed, 1);
    g13->GetXaxis()->SetTitle("#theta(deg)");
    g13->GetYaxis()->SetTitle("h");
    g13->GetXaxis()->CenterTitle();
    g13->GetYaxis()->CenterTitle();
    
    TGraph *g21 = new TGraph();
    g21->SetMarkerStyle(22);
    g21->SetMarkerColorAlpha(kGreen+1, 1);
    g21->GetXaxis()->SetTitle("#theta(deg)");
    g21->GetYaxis()->SetTitle("B");
    g21->GetXaxis()->CenterTitle();
    g21->GetYaxis()->CenterTitle();
    
    TGraph *g22 = new TGraph();
    g22->SetMarkerStyle(22);
    g22->SetMarkerColorAlpha(kGreen+1, 1);
    g22->GetXaxis()->SetTitle("#theta(deg)");
    g22->GetYaxis()->SetTitle("#mu");
    g22->GetXaxis()->CenterTitle();
    g22->GetYaxis()->CenterTitle();

    TGraph *g23 = new TGraph();
    g23->SetMarkerStyle(22);
    g23->SetMarkerColorAlpha(kRed, 1);
    g23->GetXaxis()->SetTitle("#theta(deg)");
    g23->GetYaxis()->SetTitle("m");
    g23->GetXaxis()->CenterTitle();
    g23->GetYaxis()->CenterTitle();
    
    TGraph *g31 = new TGraph();
    g31->SetMarkerStyle(22);
    g31->SetMarkerColorAlpha(kGreen+1, 1);
    g31->GetXaxis()->SetTitle("#theta(deg)");
    g31->GetYaxis()->SetTitle("C");
    g31->GetXaxis()->CenterTitle();
    g31->GetYaxis()->CenterTitle();
    
    TGraph *g32 = new TGraph();
    g32->SetMarkerStyle(22);
    g32->SetMarkerColorAlpha(kGreen+1, 1);
    g32->GetXaxis()->SetTitle("#theta(deg)");
    g32->GetYaxis()->SetTitle("#tau");
    g32->GetXaxis()->CenterTitle();
    g32->GetYaxis()->CenterTitle();

    TGraph *g33 = new TGraph();
    g33->SetMarkerStyle(22);
    g33->SetMarkerColorAlpha(kRed, 1);
    g33->GetXaxis()->SetTitle("#theta(deg)");
    g33->GetYaxis()->SetTitle("n");
    g33->GetXaxis()->CenterTitle();
    g33->GetYaxis()->CenterTitle();
    
    TGraph *g41 = new TGraph();
    g41->SetMarkerStyle(22);
    g41->SetMarkerColorAlpha(kGreen+1, 1);
    g41->GetXaxis()->SetTitle("#theta(deg)");
    g41->GetYaxis()->SetTitle("D");
    g41->GetXaxis()->CenterTitle();
    g41->GetYaxis()->CenterTitle();
    
    TGraph *g42 = new TGraph();
    g42->SetMarkerStyle(22);
    g42->SetMarkerColorAlpha(kGreen+1, 1);
    g42->GetXaxis()->SetTitle("#theta(deg)");
    g42->GetYaxis()->SetTitle("#lambda");
    g42->GetXaxis()->CenterTitle();
    g42->GetYaxis()->CenterTitle();
    
    TGraph *g43 = new TGraph();
    g43->SetMarkerStyle(22);
    g43->SetMarkerColorAlpha(kRed, 1);
    g43->GetXaxis()->SetTitle("#theta(deg)");
    g43->GetYaxis()->SetTitle("q");
    g43->GetXaxis()->CenterTitle();
    g43->GetYaxis()->CenterTitle();

    Int_t N = 0;
    for (int index_angle = 0; index_angle < N_ANGLE; index_angle++){
        Float_t theta_min = IndexToAngle(index_angle).first;
        Float_t theta_max = IndexToAngle(index_angle).second;
        Float_t theta = (theta_min+theta_max)/2.0;

        g11->SetPoint(N, theta, (*n_p1)[index_angle][0]);
        g12->SetPoint(N, theta, (*n_p1)[index_angle][1]);
        g13->SetPoint(N, theta, (*n_p1)[index_angle][2]);
        
        g21->SetPoint(N, theta, (*n_p2)[index_angle][0]);
        g22->SetPoint(N, theta, (*n_p2)[index_angle][1]);
        g23->SetPoint(N, theta, (*n_p2)[index_angle][2]);
        
        g31->SetPoint(N, theta, (*n_p3)[index_angle][0]);
        g32->SetPoint(N, theta, (*n_p3)[index_angle][1]);
        g33->SetPoint(N, theta, (*n_p3)[index_angle][2]);
        
        g41->SetPoint(N, theta, (*n_p4)[index_angle][0]);
        g42->SetPoint(N, theta, (*n_p4)[index_angle][1]);
        g43->SetPoint(N, theta, (*n_p4)[index_angle][2]);

        N++;
    }

    TF1* f11=new TF1("f11","[0]",20,140);
    f11->SetLineColor(kRed-7);
    
    TF1* f12=new TF1("f12","[0]",20,140);
    f12->SetLineColor(kRed-7);
    
    TF1* f13=new TF1("f13","[0]*(x-[1])*(x-[1])+[2]",20,140);
    f13->SetLineColor(kRed-7);
    f13->SetParameters(0.04,90,0.004);
 
    TF1* f21=new TF1("f21","[0]",20,140);
    f21->SetLineColor(kRed-7);
    
    TF1* f22=new TF1("f22","[0]",20,140);
    f22->SetLineColor(kRed-7);

    TF1* f23=new TF1("f23","[0]*(x-[1])*(x-[1])+[2]",20,140);
    f23->SetLineColor(kRed-7);
    f23->SetParameters(0.04,90,0.004);

    TF1* f31=new TF1("f31","[0]",20,140);
    f31->SetLineColor(kRed-7);

    TF1* f32=new TF1("f32","[0]",20,140);
    f32->SetLineColor(kRed-7);
    
    TF1* f33=new TF1("f33","[0]*(x-[1])*(x-[1])+[2]",20,140);
    f33->SetLineColor(kRed-7);
    f33->SetParameters(-0.04,90,0.004);

    //TF1* f41=new TF1("f41","[0]",20,140);
    //f41->SetLineColor(kRed-7);
    
    //TF1* f42=new TF1("f42","[0]",20,140);
    //f42->SetLineColor(kRed-7);
    
    TF1* f41=new TF1("f41","[0]*(x-[1])*(x-[1])+[2]",20,140);
    f41->SetLineColor(kRed-7);
    f41->SetParameters(0.04,90,0.004);

    TF1* f42=new TF1("f42","[0]*x+[1]",20,140);
    f42->SetLineColor(kRed-7);
    
    TF1* f43=new TF1("f43","[0]*(x-[1])*(x-[1])+[2]",20,140);
    f43->SetLineColor(kRed-7);
    f43->SetParameters(0.04,90,0.004);
 
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
    g43->Fit(f43,"R");

    c1->Divide(1, 3);
    c1->cd(1);
    g11->Draw("AP.");
    c1->cd(2);
    g12->Draw("AP.");
    c1->cd(3);
    g13->Draw("AP.");

    c2->Divide(1, 3);
    c2->cd(1);
    g21->Draw("AP.");
    c2->cd(2);
    g22->Draw("AP.");
    c2->cd(3);
    g23->Draw("AP.");

    c3->Divide(1, 3);
    c3->cd(1);
    g31->Draw("AP.");
    c3->cd(2);
    g32->Draw("AP.");
    c3->cd(3);
    g33->Draw("AP.");

    c4->Divide(1, 3);
    c4->cd(1);
    g41->Draw("AP.");
    c4->cd(2);
    g42->Draw("AP.");
    c4->cd(3);
    g43->Draw("AP.");

    const int precision = 3;
    cout << "ParArray1: Double_t \\ " << endl << setprecision(precision) 
    << f11->GetParameter(0) << " " << f12->GetParameter(0) << " " << f13->GetParameter(0) << " " << f13->GetParameter(1) << " " << f13->GetParameter(2) << endl;
    cout << "ParArray2: Double_t \\ " << endl << setprecision(precision)
    << f21->GetParameter(0) << " " << f22->GetParameter(0) << " " << f23->GetParameter(0) << " " << f23->GetParameter(1) << " " << f23->GetParameter(2) << endl;
    cout << "ParArray3: Double_t \\ " << endl << setprecision(precision) 
    << f31->GetParameter(0) << " " << f32->GetParameter(0) << " " << f33->GetParameter(0) << " " << f33->GetParameter(1) << " " << f33->GetParameter(2) << endl;
    cout << "ParArray4: Double_t \\ " << endl << setprecision(precision)
    //<< f41->GetParameter(0) << " " << f42->GetParameter(0) << " " << f43->GetParameter(0) << " " << f43->GetParameter(1) << " " << f43->GetParameter(2) << endl;
    << f41->GetParameter(0) << " " << f41->GetParameter(1) << " " << f41->GetParameter(2) << " " << f42->GetParameter(0) << " " << f42->GetParameter(1) << " " << f43->GetParameter(0) << " " << f43->GetParameter(1) << " " << f43->GetParameter(2)  << endl;
    
    TString picture_name1 = "doc/step3_picture/Angle_dependecy_p1.png";
    TString picture_name2 = "doc/step3_picture/Angle_dependecy_p2.png";
    TString picture_name3 = "doc/step3_picture/Angle_dependecy_p3.png";
    TString picture_name4 = "doc/step3_picture/Angle_dependecy_p4.png";
    c1->Print(picture_name1);
    c2->Print(picture_name2);
    c3->Print(picture_name3);
    c4->Print(picture_name4);

    delete c1; delete c2; delete c3; delete c4;

    delete g11; delete g12; delete g13;
    delete g21; delete g22; delete g23;
    delete g31; delete g32; delete g33;
    delete g41; delete g42; delete g43;

    delete f11; delete f12; delete f13;
    delete f21; delete f22; delete f23;
    delete f31; delete f32; delete f33;
    delete f41; delete f42; delete f43;

    return 0;
}
