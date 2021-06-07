#include "di_photon_energy.h"
typedef struct par_n{
        std::vector<Double_t> m_raw, d_m_raw, R_raw, d_Rm_raw, d_Rp_raw;
        std::vector<Double_t> m_new, d_m_new, R_new, d_Rm_new, d_Rp_new;
	Int_t theta_index;
        par_n(){
                for (int j = 0; j < N_ENERGY; j++){
                     	m_raw.push_back(.0);
                        d_m_raw.push_back(.0);
                        R_raw.push_back(.0);
                        d_Rm_raw.push_back(.0);
                        d_Rp_raw.push_back(.0);
                        m_new.push_back(.0);
                        d_m_new.push_back(.0);
                        R_new.push_back(.0);
                        d_Rm_new.push_back(.0);
                        d_Rp_new.push_back(.0);
		}

        }
}par_n;
par_n my_par;

int Exec(int index_energy);

int di_photon_energy_step1(){
        for (int j = 0; j < N_ENERGY; j++) if ( Exec(j) ) return 1;
        gROOT->cd();
        TFile f("doc/step1_di-photon_energy.root","recreate");
        TTree tree("par","par");
        tree.Branch("m_raw", &my_par.m_raw);
        tree.Branch("d_m_raw", &my_par.d_m_raw);
        tree.Branch("R_raw", &my_par.R_raw);
        tree.Branch("d_Rm_raw", &my_par.d_Rm_raw);
        tree.Branch("d_Rp_raw", &my_par.d_Rp_raw);
        tree.Branch("m_new", &my_par.m_new);
        tree.Branch("d_m_new", &my_par.d_m_new);
        tree.Branch("R_new", &my_par.R_new);
        tree.Branch("d_Rm_new", &my_par.d_Rm_new);
        tree.Branch("d_Rp_new", &my_par.d_Rp_new);
        tree.Branch("theta", &my_par.theta_index);
        tree.Fill();
        tree.Write();
        f.Close();
        return 0;
}

int Exec(int index_energy){
    gROOT->SetBatch();
    TCanvas* c1=new TCanvas("PANDA1","c1",1200,900);
    gStyle->SetOptTitle(0);
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

    static TFile* file = new TFile("doc/di-photon_energy.root", "READ");
    Float_t shower_energy = IndexToEnergy(index_energy);
    TTree *mytree = (TTree *)file->Get("A" + IndexToStr(my_angle) + "_di-photon_test_" + NumToStr(shower_energy) +"GeV");
    if ( mytree == NULL ) return 1;
    Double_t Energy_raw, Energy_new;
    Int_t index_angle;
    mytree->SetBranchAddress("Energy_raw",&Energy_raw);
    mytree->SetBranchAddress("Energy_new",&Energy_new);
    mytree->SetBranchAddress("theta",&index_angle);

    int bin1 = 100*(15.0/(0.6*shower_energy)),bin2(200);
    double xmin(0),xmax(15);

    TH1D* h1D1 = new TH1D("Hist1_1","h1_1", bin1, xmin, xmax);
    h1D1->SetLineColor(kGray+3);
    h1D1->SetLineWidth(2);
    h1D1->GetXaxis()->SetTitle("E_{#gamma}   [GeV]");
    h1D1->GetYaxis()->SetTitle("Entries");
    h1D1->GetXaxis()->CenterTitle();
    h1D1->GetYaxis()->CenterTitle();

    TH1D* h1D2 = new TH1D("Hist1_2","h1_2", bin1, xmin, xmax);
    h1D2->SetLineColor(kRed);
    h1D2->SetLineWidth(2);
    h1D2->GetXaxis()->SetTitle("E_{#gamma}   [GeV]");
    h1D2->GetYaxis()->SetTitle("Entries");
    h1D2->GetXaxis()->CenterTitle();
    h1D2->GetYaxis()->CenterTitle();

    Int_t maxEvtNo=(Int_t)mytree->GetEntries();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        mytree->GetEntry(ievt);
        if (index_angle != my_angle) continue;
        if (Energy_raw < shower_energy/3.0 || Energy_raw > 5.0*shower_energy/3.0) continue;
        if (Energy_new < shower_energy/3.0 || Energy_new > 5.0*shower_energy/3.0) continue;
        h1D1->Fill(Energy_raw);
        h1D2->Fill(Energy_new);
    }

    double NewRange_min = h1D2->GetMean()-(4.4 - 0.4*(shower_energy))*(h1D2->GetStdDev());
    double NewRange_max = h1D2->GetMean()+(4.4 - 0.4*(shower_energy))*(h1D2->GetStdDev());
                                            
    h1D2->SetAxisRange(NewRange_min, NewRange_max);
    h1D1->SetAxisRange(NewRange_min, NewRange_max);
    
    Double_t set_p00 = maxEvtNo/20.0, set_p01 = h1D1->GetMean(), set_p02 = (h1D1->GetStdDev())/10.0, set_p03 = set_p00/10.0, set_p04 =  h1D1->GetStdDev(); 
    TF1 *f1=new TF1("f1","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[1],[4])",NewRange_min+0.01*shower_energy, NewRange_max-0.01*shower_energy);
    f1->SetLineColor(kBlack);
    f1->SetParameters(set_p00,set_p01,set_p02,set_p03,set_p04);
    
    Double_t set_p10 = maxEvtNo/10.0, set_p11 = h1D2->GetMean(), set_p12 = (h1D2->GetStdDev())/10.0, set_p13 = set_p10/10.0, set_p14 =  h1D2->GetStdDev(); 
    TF1 *f2=new TF1("f2","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[1],[4])",NewRange_min+0.01*shower_energy, NewRange_max-0.01*shower_energy);
    f2->SetLineColor(kRed);
    f2->SetParameters(set_p10,set_p11,set_p12,set_p13,set_p14);
    
    h1D1->Fit(f1,"R");
    h1D2->Fit(f2,"R");
    
    c1->cd();
    h1D2->Draw();
    h1D1->Draw("SAME");
   
    TLegend * leg = new TLegend(0.7, 0.7 , 0.9, 0.8);
    leg->AddEntry(h1D1,"Bump Energy raw" , "L");
    leg->AddEntry(h1D2,"Bump Energy new", "L");
    leg->Draw();
    
    SetPar(f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2), f1->GetParameter(3), f1->GetParameter(4));
    Double_t sigma1 = finding_sigma(f1->GetParameter(1),f1->GetParameter(2)>f1->GetParameter(4)? f1->GetParameter(2):f1->GetParameter(4));
    
    SetPar(f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2)+f1->GetParError(2), f1->GetParameter(3), f1->GetParameter(4)+f1->GetParError(4));
    Double_t sigma1p = finding_sigma(f1->GetParameter(1),f1->GetParameter(2)>f1->GetParameter(4)? f1->GetParameter(2):f1->GetParameter(4));
    
    SetPar(f1->GetParameter(0), f1->GetParameter(1), f1->GetParameter(2)-f1->GetParError(2), f1->GetParameter(3), f1->GetParameter(4)-f1->GetParError(4));
    Double_t sigma1m = finding_sigma(f1->GetParameter(1),f1->GetParameter(2)>f1->GetParameter(4)? f1->GetParameter(2):f1->GetParameter(4));
    
    SetPar(f2->GetParameter(0), f2->GetParameter(1), f2->GetParameter(2), f2->GetParameter(3), f2->GetParameter(4));
    Double_t sigma2 = finding_sigma(f2->GetParameter(1),f2->GetParameter(2)>f2->GetParameter(4)? f2->GetParameter(2):f2->GetParameter(4));
    
    SetPar(f2->GetParameter(0), f2->GetParameter(1), f2->GetParameter(2)+f2->GetParError(2), f2->GetParameter(3), f2->GetParameter(4)+f2->GetParError(4));
    Double_t sigma2p = finding_sigma(f2->GetParameter(1),f2->GetParameter(2)>f2->GetParameter(4)? f2->GetParameter(2):f2->GetParameter(4));
    
    SetPar(f2->GetParameter(0), f2->GetParameter(1), f2->GetParameter(2)-f2->GetParError(2), f2->GetParameter(3), f2->GetParameter(4)-f2->GetParError(4));
    Double_t sigma2m = finding_sigma(f2->GetParameter(1),f2->GetParameter(2)>f2->GetParameter(4)? f2->GetParameter(2):f2->GetParameter(4));

    Double_t mean_OR = f1->GetParameter(1);
    Double_t mean_fix = f2->GetParameter(1);
    Double_t D_mean_OR = f1->GetParError(1);
    Double_t D_mean_fix = f2->GetParError(1);
    
    Double_t D_delta_OR_p = sigma1p - sigma1;
    Double_t D_delta_OR_m = sigma1 - sigma1m;
    Double_t D_delta_fix_p = sigma2p - sigma2;
    Double_t D_delta_fix_m = sigma2 - sigma2m;

    Double_t Resolution_OR = sigma1/mean_OR;
    Double_t Resolution_fix = sigma2/mean_fix;
    Double_t D_Resolution_OR_p = sqrt(  pow(D_delta_OR_p,2) + pow(sigma1,2)*pow(D_mean_OR,2)/pow(mean_OR,2)  )/mean_OR;
    Double_t D_Resolution_OR_m = sqrt(  pow(D_delta_OR_m,2) + pow(sigma1,2)*pow(D_mean_OR,2)/pow(mean_OR,2)  )/mean_OR;
    Double_t D_Resolution_fix_p = sqrt(  pow(D_delta_fix_p,2) + pow(sigma2,2)*pow(D_mean_fix,2)/pow(mean_fix,2)  )/mean_fix;
    Double_t D_Resolution_fix_m = sqrt(  pow(D_delta_fix_m,2) + pow(sigma2,2)*pow(D_mean_fix,2)/pow(mean_fix,2)  )/mean_fix;
    
    my_par.m_raw[index_energy] = mean_OR;
    my_par.d_m_raw[index_energy] = D_mean_OR;
    my_par.R_raw[index_energy] = Resolution_OR;
    my_par.d_Rm_raw[index_energy] = D_Resolution_OR_m;
    my_par.d_Rp_raw[index_energy] = D_Resolution_OR_p;

    my_par.m_new[index_energy] = mean_fix;
    my_par.d_m_new[index_energy] = D_mean_fix;
    my_par.R_new[index_energy] = Resolution_fix;
    my_par.d_Rm_new[index_energy] = D_Resolution_fix_m;
    my_par.d_Rp_new[index_energy] = D_Resolution_fix_p;
 
    my_par.theta_index = my_angle;
    
    cout << mean_OR << " " << D_mean_OR << " " << Resolution_OR << " " << D_Resolution_OR_m << " " << D_Resolution_OR_p << " " << mean_fix << " " << D_mean_fix << " " << Resolution_fix << " " << D_Resolution_fix_m << " " << D_Resolution_fix_p << endl;
    
    TString picture_name= "doc/step1_di-photon_picture/di-photon_energy_A" +  IndexToStr(my_angle) + "E_" + NumToStr(shower_energy) + "GeV.png";
    c1->Print(picture_name);
    
    delete c1;
    delete h1D1;
    delete h1D2;
    delete f1;
    delete f2;
    delete leg;
    return 0;
}
