Double_t Novosibirsk(Double_t x,Double_t peak=0.,Double_t width=0.,Double_t tail=0.)
{
  if (TMath::Abs(tail) < 1.e-7) {
    return TMath::Exp( -0.5 * TMath::Power( ( (x - peak) / width ), 2 ));
  }

  Double_t arg = 1.0 - ( x - peak ) * tail / width;

  if (arg < 1.e-7) {
    //Argument of logarithm negative. Real continuation -> function equals zero
    return 0.0;
  }

  Double_t log = TMath::Log(arg);
  static const Double_t xi = 2.3548200450309494; // 2 Sqrt( Ln(4) )

  Double_t width_zero = ( 2.0 / xi ) * TMath::ASinH( tail * xi * 0.5 );
  Double_t width_zero2 = width_zero * width_zero;
  Double_t exponent = ( -0.5 / (width_zero2) * log * log ) - ( width_zero2 * 0.5 );

  return TMath::Exp(exponent);
}

int Shower_instance_test(const char new_file[30], double Energy = 1.0 , int NO_Angle = 7)
{
    ostringstream out1,out2;
    out1 << NO_Angle;
    out2 << fixed << setprecision(1) << Energy;
    string str_NO_Angle = out1.str(), str_Energy = out2.str();
    std::string out_name = "doc/A"+str_NO_Angle+"_resolution_par_test.txt";
    
    int bin1 = 100*(15.0/(0.6*Energy)),bin2(200);
    float tx(1200),ty(900);
    double xmin(0),xmax(15);
    //double xmin(0),xmax(2.0);
    cout << "-INFO  New File : " << new_file << endl;
    
    string file_str(new_file);
    
    string file_name = "doc/" + file_str + ".txt";
    string file_name_min = "doc/" + file_str + "_test_min.txt";
    string file_name_max = "doc/" + file_str + "_test_max.txt";
    
    TCanvas* c1=new TCanvas("PANDA1","c1",tx,ty);
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
    
    TH1D* h1D1 = new TH1D("Hist1_1","h1_1", bin1, xmin, xmax);
    h1D1->SetLineColor(kGray+3);
    h1D1->SetLineWidth(2);
    h1D1->GetXaxis()->SetTitle("Energy");
    h1D1->GetYaxis()->SetTitle("Entries");
    h1D1->GetXaxis()->CenterTitle();
    h1D1->GetYaxis()->CenterTitle();
    
    TH1D* h1D2 = new TH1D("Hist1_2","h1_2", bin1, xmin, xmax);
    h1D2->SetLineColor(kRed);
    h1D2->SetLineWidth(2);
    h1D2->GetXaxis()->SetTitle("Energy");
    h1D2->GetYaxis()->SetTitle("Entries");
    h1D2->GetXaxis()->CenterTitle();
    h1D2->GetYaxis()->CenterTitle();
    
    TH1D* h1D3 = new TH1D("Hist1_3","h1_3", bin1, xmin, xmax);
    h1D3->SetLineColor(kGreen);
    h1D3->SetLineWidth(2);
    h1D3->GetXaxis()->SetTitle("Energy");
    h1D3->GetYaxis()->SetTitle("Entries");
    h1D3->GetXaxis()->CenterTitle();
    h1D3->GetYaxis()->CenterTitle();

    string str1, str10, str11, str2, str3;

    ifstream file1;
    file1.open(file_name, ios::in);
    Int_t MaxNo(0);
    while (getline(file1,str1)) MaxNo++;
    cout << MaxNo << endl;
    file1.clear();
    file1.seekg(0, ios::beg);
    
    ifstream file2;
    file2.open(file_name_min, ios::in);
    
    ifstream file3;
    file3.open(file_name_max, ios::in);
    
    Int_t N = 0;
    for (int i = 0; i < MaxNo/2; i++) {
        if (!getline(file1,str10)) continue;
        if (!getline(file1,str11)) continue;
        if (!getline(file2,str2)) continue;
        if (!getline(file3,str3)) continue;
        double value10= atof(str10.c_str());
        double value11= atof(str11.c_str());
        double value2= atof(str2.c_str());
        double value3= atof(str3.c_str());
        h1D1->Fill(value10);
        h1D1->Fill(value11);
        h1D2->Fill(value2);
        h1D3->Fill(value3);
        N++;
    }
    file1.close();
    file2.close();
    file3.close();
    cout << "Entries : " << N << endl;
    
    double NewRange_min = h1D1->GetMean()-(4.4 - 0.4*(Energy))*(h1D1->GetStdDev());
    double NewRange_max = h1D1->GetMean()+(4.4 - 0.4*(Energy))*(h1D1->GetStdDev());
                                            
    h1D2->SetAxisRange(NewRange_min, NewRange_max);
    h1D1->SetAxisRange(NewRange_min, NewRange_max);
    h1D3->SetAxisRange(NewRange_min, NewRange_max);
    
    c1->cd();
    h1D1->Draw();
    h1D2->Draw("SAME");
    h1D3->Draw("SAME");
   
    TLegend * leg = new TLegend(0.7,0.7 , 0.9, 0.8);
    leg->AddEntry(h1D1, "Bump Energy", "L");
    leg->AddEntry(h1D2, "Bump Energy min", "L");
    leg->AddEntry(h1D3, "Bump Energy max", "L");
    //leg->AddEntry(h1D1,"Bump Energy old" , "L");
    //leg->AddEntry(h1D2,"Bump Energy new", "L");
    leg->Draw("SAME");
    
    return 0;
}
