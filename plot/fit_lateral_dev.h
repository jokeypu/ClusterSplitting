const Double_t RM = 2.00;
typedef std::pair<Float_t, Float_t> Range;

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

const int N_ENERGY = 19;
//const int N_ENERGY = 15;
Float_t IndexToEnergy(int index){
        //Float_t my_energy[N_ENERGY]={0.2, 0.4, 0.6, 0.8, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0};
	Float_t my_energy[N_ENERGY]={
        .2, .3, .4, .5, .6, .7, .8, .9,
	1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0
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
