const Double_t RM = 2.00;
typedef std::pair<Float_t, Float_t> Range;
Double_t par[5];
int my_angle = 15;

const int N_ANGLE = 23;
Range IndexToAngle(int index){
        Float_t my_range[N_ANGLE][2]={
        {23.8514, 24.6978},{26.4557, 27.3781},{29.4579, 30.4916},{32.6536, 33.7759},{36.1172, 37.3507},
        {39.9051, 41.2390},{44.2385, 45.7355},{48.8451, 50.4459},{53.7548, 55.4790},{59.0059, 60.8229},
        {64.7855, 66.7591},{70.8088, 72.8652},{77.0506, 79.1942},{83.4997, 85.6749},{90.2068, 92.4062},
        {96.8200, 99.0099},{103.361, 105.534},{109.793, 111.893},{116.067, 118.019},{121.838, 123.686},
        {127.273, 129.033},{132.400, 134.031},{137.230, 138.679}
        };
        return make_pair(my_range[index][0],my_range[index][1]);
}

const int N_ENERGY = 9;
Float_t IndexToEnergy(int index){
        Float_t my_energy[N_ENERGY]={
        2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0
        };
        return my_energy[index];
}
TString NumToStr(Float_t num){
        ostringstream out;
        out << fixed << setprecision(1) << num;
        return (TString)out.str();
}
TString IndexToStr(int index){
        ostringstream out;
        out << index;
        return (TString)out.str();
}

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

void SetPar(Double_t par0, Double_t par1, Double_t par2, Double_t par3, Double_t par4){
    par[0] = par0;
    par[1] = par1;
    par[2] = par2;
    par[3] = par3;
    par[4] = par4;
}
Double_t fit_func(Double_t x){
    return par[0]*TMath::Gaus(x,par[1],par[2])+par[3]*TMath::Gaus(x,par[1],par[4]);
}

Double_t func_Int(Double_t mean, Double_t sigma){
    ROOT::Math::IntegratorOneDimOptions::SetDefaultAbsTolerance(1.E-6);
    ROOT::Math::Functor1D wf(&fit_func);
    ROOT::Math::Integrator ig(ROOT::Math::IntegrationOneDim::kGAUSS);
    ig.SetFunction(wf);
    Double_t value = ig.Integral(mean-sigma,mean+sigma);
    return value;
}

Double_t finding_sigma(Double_t mean, Double_t Init_sigma){
    double p = 0.0001;
    double cut = 0.6826 * func_Int(mean, 0.5);
    double sigma_shift = 9999;
    double sigma_max = 2*Init_sigma;
    double sigma_min = 0.0001;
    while (sigma_shift > 0.000001) {
        double value_min = func_Int(mean, sigma_min);
        double value_max = func_Int(mean, sigma_max);
        double sigma_mid = (sigma_max+sigma_min)/2.0;
        double value_mid = func_Int(mean, sigma_mid);
        if (cut < value_mid) sigma_max = sigma_mid;
        else sigma_min = sigma_mid;
        sigma_shift = fabs(sigma_max-sigma_min);
    }
    return (sigma_max+sigma_min)/2.0;
}
