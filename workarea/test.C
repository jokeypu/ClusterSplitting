int test(){
TCanvas *c1 = new  TCanvas("c1","just a title",1);
TF2 *f2 = new TF2("saddle","-0.57*exp(-8.46*x)+5.59e-5*pow(y-97.54,2)+2.59",-0,6,22,140);
f2->SetNpy(100);
f2->SetNpx(100);
f2->Draw("surf1");



return 0;
}
