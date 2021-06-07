#include "fit_lateral_dev.h"
typedef struct par{
	std::vector< std::vector<Double_t> > par1, par2, par3, par4;
	par(){
		for (int i = 0; i < N_ANGLE; i++){
			std::vector<Double_t> temp;
			for (int j = 0; j < N_ENERGY; j++) temp.push_back(.0);
			par1.push_back(temp);
			par2.push_back(temp);
			par3.push_back(temp);
			par4.push_back(temp);
		}
		
	}
}par;
par my_par;

Double_t lateral_dev_func(Double_t r,Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4){
    Double_t xi = r - p2*r*exp(-pow(r/p3/RM,p4));
    return p0*exp(-p1*xi/RM);
}

int Exec(int index_angle, int index_energy);

int fit_lateral_dev_step1(){
	for (int i = 0; i < N_ANGLE; i++){
		for (int j = 0; j < N_ENERGY; j++) if ( Exec(i,j) ) return 1;
	} 
	gROOT->cd();
	TFile f("doc/step1_fit_par.root","recreate");
	TTree tree("par","par");
        tree.Branch("p1", &my_par.par1);
        tree.Branch("p2", &my_par.par2);
        tree.Branch("p3", &my_par.par3);
        tree.Branch("p4", &my_par.par4);
	tree.Fill();
	tree.Write();
	f.Close();
	return 0;
}

int Exec(int index_angle, int index_energy){   
    gROOT->SetBatch();
    TCanvas* c1 = new TCanvas("c1","Lateral development",800,600);
    TCanvas* c2 = new TCanvas("c2","Lateral development - G",800,600);
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
    gStyle->SetOptFit(1111);
    //gStyle->SetPalette(1);
    
    c1->SetLeftMargin(0.13);
    c1->SetRightMargin(0.13);
    c1->SetTopMargin(-0.13);
    c1->SetBottomMargin(0.13);

    c2->SetLeftMargin(0.13);
    c2->SetRightMargin(0.13);
    c2->SetTopMargin(-0.13);
    c2->SetBottomMargin(0.13);

    static TFile* file = new TFile("doc/fit_lateral_dev.root", "READ");
    Float_t shower_energy = IndexToEnergy(index_energy);
    Float_t theta_min = IndexToAngle(index_angle).first;
    Float_t theta_max = IndexToAngle(index_angle).second;
    TTree *mytree = (TTree *)file->Get("photon_" + NumToStr(shower_energy) +"GeV");
    if ( mytree == NULL ) return 1;
    Double_t Energy, r, theta;
    mytree->SetBranchAddress("Energy",&Energy);
    mytree->SetBranchAddress("r",&r);
    mytree->SetBranchAddress("theta",&theta);
    
    const Float_t r_cut =3.5;
    const Int_t binx = 250;
    const Int_t biny = 250;
    TH2D* h = new TH2D("Hist", Form("%.1fGeV & #theta %.2f^{o}", shower_energy, (theta_min+theta_max)/2.0), binx, 0, r_cut, biny, 0, 1);
    h->SetMarkerStyle(7);
    h->SetMarkerColorAlpha(kAzure+3, 1);
    h->GetYaxis()->SetTitle("E_{target}/E_{Shower}");
    h->GetXaxis()->SetTitle("r   [cm]");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetZaxis()->CenterTitle();

    TH2D* h_cp = new TH2D("Hist_cp", Form("%.1fGeV & #theta %.2f^{o}", shower_energy, (theta_min+theta_max)/2.0), binx, 0, r_cut, biny, 0, 1.2);
    h_cp->SetMarkerStyle(7);
    h_cp->SetMarkerColorAlpha(kAzure+3, 1);
    h_cp->GetYaxis()->SetTitle("E_{target}/E^{'}_{seed}");
    h_cp->GetXaxis()->SetTitle("r   [cm]");
    h_cp->GetXaxis()->CenterTitle();
    h_cp->GetYaxis()->CenterTitle();
    h_cp->GetZaxis()->CenterTitle();

    TGraph* g = new TGraph();
    g->SetMarkerStyle(20);
    g->SetMarkerColor(kYellow);
    g->GetYaxis()->SetTitle("E_{target}/E_{Shower}");
    g->GetXaxis()->SetTitle("r   [cm]");
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();

    TGraph* g_cp = dynamic_cast<TGraph*>(g->Clone());
    Int_t maxEvtNo=(Int_t)mytree->GetEntries();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
	mytree->GetEntry(ievt);
	theta *= TMath::RadToDeg();
        if ( theta > theta_max || theta < theta_min || r > r_cut ) continue;
        //if (r < 1 && Energy/shower_energy < 0.2 ) continue;   
        h->Fill(r, Energy/shower_energy);   
    }

    int cunt = 0;
    int step = 9;
    for (int i = 5; i < binx+1; i+=step){
        Double_t mean = h->ProfileY("px",i,i+step-1)->GetMean();
        Double_t wx = r_cut/binx;
        Double_t nx = (i-1+step/2)*wx;
        g->SetPoint(cunt, nx, mean);
        cunt++;
    }

    //for (int i = 1; i <= binx; i++){
    //    for (int j = 1; j <= biny; j++) if (h->GetBinContent(i,j)<2) h->SetBinContent(i,j,0);
    //}

    TF1* f = new TF1("f","lateral_dev_func(x,[0],[1],[2],[3],[4])",0,r_cut);
    f->SetParameters(0.75, 2.78, 1.15, 0.9, 3);
    f->SetLineWidth(3);
    f->SetLineColor(kRed);

    c1->cd();
    h->Draw("PCOLZ");
    g->Draw("Psame");
    g->Fit(f,"R");
    
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        mytree->GetEntry(ievt);
	theta *= TMath::RadToDeg();
        if ( theta > theta_max || theta < theta_min || r > r_cut ) continue;
        //if (r < 1 && Energy/shower_energy < 0.2 ) continue;   
        h_cp->Fill(r, Energy/shower_energy/f->GetParameter(0));
    }
    
    cunt = 0;
    for (int i = 5; i < binx+1; i+=step){
        Double_t mean = h_cp->ProfileY("px",i,i+step-1)->GetMean();
        Double_t wx = r_cut/binx;
        Double_t nx =( (i+(step-1)/2)*wx - wx/2  );
        g_cp->SetPoint(cunt, nx, mean);
        cunt++;
    }
    f->SetParameter(0,1.0);
    c2->cd();
    h_cp->Draw("PCOLZ");
    g_cp->Draw("Psame");
    f->Draw("SAME");

    TString picture_name= "doc/step1_picture/lateral_dev_A" +  IndexToStr(index_angle) + "_" + NumToStr(shower_energy) + "GeV.png";
    c2->Print(picture_name);

    my_par.par1[index_angle][index_energy] = f->GetParameter(1);
    my_par.par2[index_angle][index_energy] = f->GetParameter(2);
    my_par.par3[index_angle][index_energy] = f->GetParameter(3);
    my_par.par4[index_angle][index_energy] = f->GetParameter(4);

    delete c1;
    delete c2;
    delete h;
    delete g;
    delete f;
    delete h_cp;
    delete g_cp;
    return 0;
}
