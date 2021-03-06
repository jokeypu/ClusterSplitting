struct myfunc {
    std::vector<double> par1 = {8.62369, 5.32678, 12.0, 0.696003, 22.8543};
    std::vector<double> par2 = {83.4757, -0.171518, 1.88588, 0.199225, 23.0441};
    std::vector<double> par3 = {0.0227033, 4.97094, 0.772748, 4.0, 0.0018315};
    std::vector<double> par4 = {0.00790974, 1.56139, 1.39, 0.428328, 0.00115601};
    std::vector<double> par5 = {-0.0849149, 2.58396, 0.419646};
    std::vector<double> par6 = {10, 0.0124098, 0.0780432, 0.182612};
    std::vector<double> par7 = {0.144939, -0.435278, 0.0642399};
    Double_t func_x0(Double_t d){
        if (d < 1.7) return par1[0]*TMath::Vavilov(d - par1[1], par1[2], par1[3]) + par1[4];
        else return par2[0]*TMath::Landau(d - par2[1], par2[2], par2[3]) + par2[4];
    }
    Double_t func_a(Double_t d){
        if (d < 1.39) return par3[0]*TMath::Poisson(par3[1]*(d-par3[2]), par3[3])+par3[4];
        else return par4[0]*TMath::Poisson(par4[1]*(d-par4[2]), par4[3])+par4[4];
    }
    Double_t func_h(Double_t d){
        if (d < 1.4 ) return par5[0]*pow(d,par5[1])+par5[2];
        else if ( d < 3.5) return par6[0]*TMath::Landau(d-par6[1],par6[2],par6[3]);
        else return par7[0]*exp(par7[1]*d+par7[2]);
    }
    Double_t m(Double_t d, Double_t angle){
        Double_t h = func_h(d);
        if ((d < 0.8) || (d > 2.8)) return h;
        else{
            Double_t a = func_a(d);
            Double_t x0 = func_x0(d);
            a *= a;
            angle = abs(fmod(angle,45.0) - 45*(((int)(angle/45.0))%2));
            if (angle<x0) return a*angle*angle+h;
            else {cout<< "Angle:" << angle << ", " << a*x0*x0+(a/(45/x0-1))*(x0-45)*(x0-45)-(a/(45/x0-1))*(angle-45)*(angle-45)<<
                "+" << h << endl;return a*x0*x0+(a/(45/x0-1))*(x0-45)*(x0-45)-(a/(45/x0-1))*(angle-45)*(angle-45)+h;}
        }
    }
    Double_t m(Double_t distance, Double_t angle, Double_t par){
        if ( angle > 90 && angle <= 180 ) angle = 180 -angle;
        if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
        if ( distance < 4 ) {
            Double_t value = (1.585 - 0.0237*angle)*pow(distance,5) - (1.371/(angle + 24.672))*(pow(distance,4)+22.845*pow(distance,3) - 3.794*distance*distance) + 0.485*distance + 2.787;
            return 2.456/value;
        }else return exp(-1* par * distance);
    }
}func;

std::vector<double> par1 = {1.47994, 1.40911, 1.5216, 1.58767, 1.41823, 1.20107, 1.40021, 1.61076, 1.55799, 1.69814, 1.44313, 1.36737, 1.44847, 1.50126, 1.30191, 1.50295, 1.68052, 1.53869, 1.32379, 1.44949, 1.50036, 1.31779, 1.28991, 1.55822, 1.37487, 1.11861, 1.59666, 1.35665, 1.42687, 1.45577, 1.45591, 1.21475, 1.00299, 1.30838, 1.05776, 1.46028, 1.20945, 1.12975, 1.29005, 1.12541, 1.30164, 0.89004, 1.03526, 1.04499, 1.42351, 1.2598, 1.15151, 1.03511, 1.35549, 0.858096, 1.10612, 0.965125, 1.10845, 0.89562, 1.03077, 1.13563, 1.2431, 1.12137, 0.936256, 0.68764, 1.19257, 0.78799, 0.954587, 0.830432, 1.04553, 1.13091, 0.89148, 0.896716, 1.04182, 0.999725, 0.827203, 0.66813, 0.768195, 0.789239, 0.880294, 0.748822, 0.749049, 0.898385, 0.917733, 0.657576, 0.76885, 1.00577, 1.06092, 0.509508, 0.783106, 0.973449, 0.847542, 0.758128, 0.721299, 0.631344, 0.874684, 0.83243, 0.855176, 0.782924, 0.834866, 0.834289, 0.772338, 0.848204, 0.804116, 0.765132, 0.716634, 0.720747, 0.905901, 0.669254, 0.781137, 0.995052, 0.908544, 0.619489, 0.725752, 0.841246, 0.773001, 0.782085, 0.520848, 0.682983, 0.678196, 0.635669, 0.632214, 0.783848, 0.729467, 0.820897, 0.728379, 0.695053, 0.722519, 0.580005, 0.661338, 0.653723, 0.594421, 0.468908, 0.438184, 0.535993, 0.572852, 0.742575, 0.579848, 0.495763, 0.601748, 0.539577, 0.475001, 0.616259, 0.478793, 0.499877, 0.722398, 0.495558, 0.3571, 0.441352, 0.628777, 0.609481, 0.513991, 0.588428, 0.601055, 0.446958, 0.622156, 0.49616, 0.643373, 0.458008, 0.656871, 0.625916, 0.608822, 0.562916, 0.671652, 0.582634, 0.539649, 0.787993, 0.579912, 0.627778, 0.583095, 0.526202, 0.710461, 0.635143, 0.585342, 0.635929, 0.639902, 0.690184, 0.724497, 0.55786, 0.688477, 0.630092, 0.549355, 0.643222, 0.721614, 0.640634, 0.600949, 0.68907, 0.674864, 0.759479, 0.655113, 0.828516, 0.673711, 0.944664, 0.767714, 0.720903, 0.701229, 0.850507, 0.677267, 0.694624, 0.8888, 0.762769, 0.658335, 0.791415, 0.78603, 0.627016, 0.651832, 0.873996, 0.882778, 0.722487, 0.756483, 0.607489, 0.840972, 0.679022, 1.00984, 0.766365, 0.762409, 0.851544, 1.05027, 0.871671, 0.909959, 1.05971, 0.873106, 0.78498, 0.85526, 0.854952, 0.893633, 0.759977, 1.11885, 0.726261, 1.10145, 1.00594, 0.936423, 0.898264, 0.566903, 1.01108, 0.978162, 0.97062, 0.972432, 0.790211, 0.949527, 0.677132, 0.849114, 1.20114, 1.02572, 0.829375, 1.01202, 0.856844, 1.00655, 0.908292, 1.10332, 1.0529, 1.01916, 1.13544, 0.833919, 1.05235, 0.950042, 1.24953, 0.837041, 1.34852, 1.24002, 0.927093, 1.34141, 1.26009, 1.38737, 1.37204, 1.05675, 1.21017, 1.07819, 1.33668, 1.20768, 1.26512, 1.4366, 1.21145, 1.31817, 1.25951, 1.31265, 1.18841, 1.41653, 1.12476, 1.17037, 1.31719, 1.2444, 1.24971, 1.26595, 1.42063, 1.32398, 1.37531, 1.41557, 1.48662, 1.42628, 1.4907, 1.47549, 1.47494, 1.3504, 1.34299, 1.51453, 1.41799, 1.37948, 1.3809, 1.30753, 1.51892, 1.56084, 1.29576, 1.47021, 1.52589, 1.52589};
std::vector<double> par2 = {2.66896, 2.49278, 2.80345, 2.89587, 2.53116, 2.03032, 2.43975, 2.95661, 2.77252, 3.15394, 2.55249, 2.42622, 2.65767, 2.72616, 2.2347, 2.73141, 3.05106, 2.86981, 2.20703, 2.63139, 2.70011, 2.28799, 2.30774, 2.83522, 2.47116, 1.8769, 2.91516, 2.45296, 2.65155, 2.65813, 2.78106, 2.14554, 1.55177, 2.2633, 1.7169, 2.78115, 2.04959, 2.00416, 2.30041, 1.95049, 2.31355, 1.22492, 1.56356, 1.79032, 2.63897, 2.28102, 2.0282, 1.67633, 2.45868, 1.35526, 1.82565, 1.61954, 2.09909, 1.51749, 1.82267, 2.06525, 2.39333, 2.03185, 1.64258, 0.95157, 2.18628, 1.2758, 1.62803, 1.32005, 1.86366, 2.11769, 1.40808, 1.4002, 1.76142, 1.76, 1.36814, 1.00798, 1.15659, 1.35867, 1.468, 1.31071, 1.13268, 1.52369, 1.51906, 0.939691, 1.28152, 1.83448, 2.05368, 0.637378, 1.35827, 1.90374, 1.4623, 1.23614, 1.15256, 0.95972, 1.66724, 1.40318, 1.54213, 1.36875, 1.56704, 1.51898, 1.40027, 1.56842, 1.53894, 1.21049, 1.33156, 1.24108, 1.62466, 1.06124, 1.47042, 1.91208, 1.81024, 1.03995, 1.36047, 1.58137, 1.47129, 1.45371, 0.827003, 1.12369, 1.27271, 1.11212, 1.17264, 1.55889, 1.45651, 1.63882, 1.3695, 1.34007, 1.40464, 1.01638, 1.27249, 1.20306, 1.13775, 0.840592, 0.743743, 0.94896, 0.978785, 1.41735, 1.10671, 0.927937, 1.15159, 1.0082, 0.870135, 1.16996, 0.869439, 0.924373, 1.46248, 0.926641, 0.465128, 0.73435, 1.325, 1.185, 0.953477, 1.13338, 1.1283, 0.785783, 1.20098, 0.853101, 1.34885, 0.754249, 1.29577, 1.27105, 1.22525, 1.1137, 1.37496, 1.04011, 0.967997, 1.6301, 1.05949, 1.25039, 1.13691, 0.992427, 1.48312, 1.1932, 1.08105, 1.2027, 1.2287, 1.34198, 1.4273, 1.0085, 1.35113, 1.2724, 0.949598, 1.26141, 1.43119, 1.13857, 1.1127, 1.31641, 1.26746, 1.46623, 1.2242, 1.63115, 1.18876, 1.90111, 1.48521, 1.39906, 1.3169, 1.63189, 1.18844, 1.23203, 1.71187, 1.34889, 1.15075, 1.46681, 1.47676, 0.959881, 1.04886, 1.60121, 1.62965, 1.31811, 1.37114, 0.941688, 1.53951, 1.12537, 1.97132, 1.31065, 1.28464, 1.58935, 2.00845, 1.5215, 1.67351, 1.92489, 1.62972, 1.39437, 1.48894, 1.52074, 1.57092, 1.19209, 2.12267, 1.11734, 2.16532, 1.68846, 1.66906, 1.4942, 0.745874, 1.77681, 1.7075, 1.79041, 1.71171, 1.20515, 1.47032, 0.974393, 1.38426, 2.1419, 1.75069, 1.4106, 1.67161, 1.32163, 1.70253, 1.57238, 1.98548, 1.82058, 1.75653, 2.05942, 1.38215, 1.89048, 1.54624, 2.25449, 1.15548, 2.40144, 2.29742, 1.47754, 2.45524, 2.20288, 2.51807, 2.51233, 1.72627, 2.18322, 1.80719, 2.45504, 2.11215, 2.24465, 2.6528, 2.10092, 2.28634, 2.17699, 2.26368, 2.04988, 2.53467, 1.91532, 2.07669, 2.30477, 2.16611, 2.21703, 2.11469, 2.61702, 2.29666, 2.38737, 2.49955, 2.71136, 2.58961, 2.748, 2.67885, 2.72404, 2.40714, 2.41498, 2.78601, 2.5474, 2.45347, 2.44354, 2.23669, 2.67769, 2.81414, 2.29152, 2.67774, 2.76884, 2.76884};
Double_t newfunc3(const TVector3 *DetPos, const TVector3 *Cent, const Double_t par){
    Double_t distance(0), angle(0);
    if (*DetPos != *Cent) {
        TVector3 vz(0, 0, 1);
        TVector3 DetPos_o = (*DetPos) - 3.7*vz;
        TVector3 DetPos_n;
        DetPos_n.SetMagThetaPhi(DetPos_o.Mag(), DetPos_o.Theta(), DetPos_o.Phi()-0.06981317);
        TVector3 ey = DetPos_n.Cross(vz).Unit();
        TVector3 ex = DetPos_n.Cross(ey).Unit();
        Double_t dx = abs((*Cent-*DetPos).Dot(ex));
        Double_t dy = abs((*Cent-*DetPos).Dot(ey));
        distance = sqrt(dx*dx+dy*dy);
        angle = 57.2957*TMath::ATan(dy/dx);
        if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
        if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    }
    //distance = (distance-1.66);
    //if (distance<0) distance = 0;
    
    Int_t N_angle = (Int_t) (angle / 0.3);
    if ( distance < -3.3 ) {
        Double_t value = (par1[N_angle]*distance*distance*distance*distance*distance - par2[N_angle]*(distance*distance*distance*distance - 0.865*distance*distance*distance + 0.349*distance*distance) + 0.241*distance +1.133);
        return 1/value;
    }else return exp(-1* par * distance);  //0.2*
}

Double_t rat(const TVector3 *DetPos_i, const TVector3 *DetPos_0, const TVector3 *Cent, const Double_t par) {
    Double_t distance(0);
    if (*DetPos_i != *Cent) {
        TVector3 vz(0, 0, 1);
        TVector3 DetPos_o = (*DetPos_i) - 3.7*vz;
        TVector3 DetPos_n;
        DetPos_n.SetMagThetaPhi(DetPos_o.Mag(), DetPos_o.Theta(), DetPos_o.Phi()-0.06981317);
        TVector3 ey = DetPos_n.Cross(vz).Unit();
        TVector3 ex = DetPos_n.Cross(ey).Unit();
        Double_t dx = abs((*Cent-*DetPos_i).Dot(ex));
        Double_t dy = abs((*Cent-*DetPos_i).Dot(ey));
        distance = sqrt(dx*dx+dy*dy);
    }
    Double_t value;
    if (distance < -1.5)
    value = newfunc3(DetPos_i, Cent, par)/newfunc3(DetPos_0, Cent, par);
    else value = newfunc3(DetPos_i, Cent, par);
    //std::cout << "value:" << value << std::endl;
    //if ( value >= 1.0 ) return 0.99;
    return value;
}
Double_t DD(const TVector3 *DetPos, const TVector3 *Cent, const Double_t par){
    Double_t distance(0), angle(0);
    if (*DetPos != *Cent) {
        TVector3 vz(0, 0, 1);
        TVector3 DetPos_o = (*DetPos) - 3.7*vz;
        TVector3 DetPos_n;
        DetPos_n.SetMagThetaPhi(DetPos_o.Mag(), DetPos_o.Theta(), DetPos_o.Phi()-0.06981317);
        TVector3 ey = DetPos_n.Cross(vz).Unit();
        TVector3 ex = DetPos_n.Cross(ey).Unit();
        Double_t dx = abs((*Cent-*DetPos).Dot(ex));
        Double_t dy = abs((*Cent-*DetPos).Dot(ey));
        distance = sqrt(dx*dx+dy*dy);
        angle = 57.2957*TMath::ATan(dy/dx);
        if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
        if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    }
    return distance;
}
Double_t AA(const TVector3 *DetPos, const TVector3 *Cent, const Double_t par){
    Double_t distance(0), angle(0);
    if (*DetPos != *Cent) {
        TVector3 vz(0, 0, 1);
        TVector3 DetPos_o = (*DetPos) - 3.7*vz;
        TVector3 DetPos_n;
        DetPos_n.SetMagThetaPhi(DetPos_o.Mag(), DetPos_o.Theta(), DetPos_o.Phi()-0.06981317);
        TVector3 ey = DetPos_n.Cross(vz).Unit();
        TVector3 ex = DetPos_n.Cross(ey).Unit();
        Double_t dx = abs((*Cent-*DetPos).Dot(ex));
        Double_t dy = abs((*Cent-*DetPos).Dot(ey));
        distance = sqrt(dx*dx+dy*dy);
        angle = 57.2957*TMath::ATan(dy/dx);
        if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
        //if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    }
    return angle;
}

Double_t p[7];
void SetPar(Double_t Rm, Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5, Double_t p6){
        p[0] = Rm;
        p[1] = p1;
        p[2] = p2;
        p[3] = p3;
        p[4] = p4;
        p[5] = p5;
        p[6] = p6;
}
Double_t f(const Double_t *x){
    Double_t d = sqrt(x[0]*x[0]+x[1]*x[1])+0.0001;
       //return exp(-1*c[0]*d/c[3])+c[1]*exp(-1*c[2]*d/c[3]);
    return (p[1]*exp(-1*p[2]*d)+p[3]*exp(-1*p[4]*d)+p[5]*exp(-1*p[6]*d))/(3816*0.2*d*TMath::TwoPi());
}
Double_t mf(Double_t distance, Double_t angle, Double_t L0 = 1.064, Double_t p1 = 1404.71, Double_t p2 = 3.15506, Double_t p3 = 169.204, Double_t p4 = 0.887089, Double_t p5 = 45.4251, Double_t p6 = 0.354403){
    SetPar(2.0,p1,p2,p3,p4,p5,p6);
    angle *= 0.017453;
    //double L0 = 1.0;
    double a[2] = {distance*cos(angle)-L0,distance*sin(angle)-L0};
    double b[2] = {distance*cos(angle)+L0,distance*sin(angle)+L0};
    const double ERRORLIMIT = 1E+0;
    ROOT::Math::Functor wf(&f,2);
    ROOT::Math::IntegratorMultiDim ig(ROOT::Math::IntegrationMultiDim::kADAPTIVE);
    ig.SetFunction(wf);
    return ig.Integral(a,b);
}

Double_t Int(Double_t a,Double_t b,Double_t p){
    int n = 3;
    Double_t ka = (a/TMath::PiOver2())-round(a/TMath::PiOver2());
    Double_t kb = (b/TMath::PiOver2())-round(b/TMath::PiOver2());
    Double_t sum1(0),sum2(0),x(a);
    Double_t step = (b-a)/(2*n);
    for (int i = 1; i < 2*n; i+=2) sum1 += exp(-1*p/sin(x+i*step));
    for (int i = 2; i < 2*n-1; i+=2) sum2 += exp(-1*p/sin(x+i*step));
    return ((b-a)/(6*n))*(exp(-1*p/sin(a))+exp(-1*p/sin(b))+4*sum1+2*sum2);
}

Double_t Func(Double_t d,Double_t a,Double_t fPar,Double_t L0){
    if ( a > 90 && a <= 180 ) a = 180 - a;
    if ( a > 45 && a <= 90 ) a = 90 - a;
    a *= TMath::DegToRad();
    Double_t x0 = d*cos(a), y0 = d*sin(a);
    Double_t xp = x0+L0 , xm = x0-L0, yp = y0+L0, ym = y0-L0;
    TVector2 point1(xp,ym), point2(xp,yp),  point3(xm,yp), point4(xm,ym);
    Double_t a1 = point1.Phi(), a2 = point2.Phi(), a3 = point3.Phi(), a4 = point4.Phi();
    int w3(-1),w4(-1);
    if (xm>=0 && ym<0) w4 = 1;
    else if (xm<0 && ym<=0) w3 = w4 = 1;
    if (fabs(a2 - a1) > TMath::Pi()) a2<a1 ? a1 -= TMath::TwoPi() : a1 += TMath::TwoPi();
    Double_t result1 = a2 - a1 - Int(a1+TMath::PiOver2(), a2+TMath::PiOver2(), fPar*xp);
    if (fabs(a3 - a2) > TMath::Pi()) a3<a2 ? a2 -= TMath::TwoPi() : a2 += TMath::TwoPi();
    Double_t result2 = a3 - a2 - Int(a2, a3, fPar*yp);
    if (fabs(a4 - a3) > TMath::Pi()) a4<a3 ? a3 -= TMath::TwoPi() : a3 += TMath::TwoPi();
    Double_t result3;
    //if (fabs(xm) < 0.15) result3=0.0;
    //result3 = fabs(xm)*(a3 - a4 - Int(-TMath::ATan(L0/(2*0.15))+TMath::PiOver2(), TMath::ATan(L0/(2*0.15))+TMath::PiOver2(), fPar*0.15))/0.15;
    if (a4 < a3) result3 = a3 - a4 - Int(a4+TMath::PiOver2(), a3+TMath::PiOver2(), fPar*xm);
    else result3 = a4 - a3 - Int(a3+TMath::PiOver2(), a4+TMath::PiOver2(), fPar*xm);
    if (fabs(a1 - a4) > TMath::Pi()) a1<a4 ? a4 -= TMath::TwoPi() : a4 += TMath::TwoPi();
    Double_t result4;
    if (a1 < a4 ) result4 = a4 - a1 - Int(a1, a4, fPar*ym);
    else result4 = a1 - a4 - Int(a4, a1, fPar*ym);
    return (result1+result2+w3*result3+w4*result4)/fPar;
}

Double_t result(Double_t distance,Double_t angle, Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5, Double_t p6 ){
    //time_t begin,end;
    //begin = clock();
    Double_t p[7] = {p0, p1, p2, p3, p4, p5, p6};
    //Double_t p[7] = {1.22069, 37147,  168.255,  -15302.4, 44.5161, 1489.1, 1.74012};
    Double_t value =(p[1]*Func(distance,angle,p[2],p[0])+p[3]*Func(distance,angle,p[4],p[0])+p[5]*Func(distance,angle,p[6],p[0]))/(3816*0.2*TMath::TwoPi());
    //end = clock();
    //cout << "TIME:" << end - begin << endl;
    return value;
}

Double_t NFunc(Double_t x,Double_t y,Double_t fPar,Double_t L0){
    Double_t x0, y0;
    if (fabs(x)<fabs(y)) {x0 = fabs(y);y0 = fabs(x);}
    else {x0 = fabs(x);y0 = fabs(y);}
    Double_t xp = x0+L0 , xm = x0-L0, yp = y0+L0, ym = y0-L0;
    Double_t min_distance = xm, max_distance = sqrt(xp*xp+yp*yp);
    if (xm>0 && ym>0) min_distance = sqrt(xm*xm+ym*ym);
    else if (xm<0) {
        return 2*L0*L0*(1-exp(-fPar*max_distance))/(max_distance*max_distance*fPar);
        //return TMath::TwoPi()*(1-exp(-fPar*max_distance))/fPar;
        //min_distance = fabs(xm);
        //return TMath::TwoPi()*(1-exp(-fPar*min_distance)+(exp(-fPar*min_distance)-exp(-fPar*max_distance)) * (L0*L0/TMath::Pi()-min_distance*min_distance)/(max_distance*max_distance-min_distance*min_distance))/fPar;
    }
    return 2*L0*L0*(exp(-fPar*min_distance)-exp(-fPar*max_distance))/((max_distance+min_distance)*(max_distance-min_distance)*fPar);
}

Double_t loop(Double_t distance, Double_t angle, Double_t fPar, Double_t L0){
    Int_t N = 2;
    if (distance < 3*L0){
        if (distance < 1.42*L0) N = 18;
        else N = 10;
    }
    //if (distance <= L0*sqrt(2)) N = (int)25*exp(-0.3*fabs(distance));
    //if (distance <= L0/2) N = 25;
    Double_t x0 = distance*cos(angle), y0 = distance*sin(angle);
    Double_t xp = x0+L0 , xm = x0-L0, yp = y0+L0, ym = y0-L0;
    //N = (int)50*exp(-(1.2-angle/80)*fabs(x0));
    //else N = (int)30*exp(-(1.2-angle/80)*fabs(x0));
    //if (distance < 3*L0  &&  distance > L0*sqrt(2)) N = (int)8*exp(-0.8*distance);
    //if (distance <= L0*sqrt(2)) N = (int)50*exp(-0.8*fabs(xm));
    Double_t step = 2*L0/N, step_over2 = L0/N, sum = 0;
    Double_t xi = xm+step_over2;
    for (int i = 0; i < N; i++){
        Double_t yi = ym+step_over2;
        for (int j = 0; j < N; j++){
            sum += NFunc(xi,yi,fPar,step_over2);
            yi += step;
        }
        xi += step;
    }
    return sum;
}

Double_t Value(Double_t distance,Double_t angle, Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5, Double_t p6){
    //time_t begin,end;
    //begin = clock();
    if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
    if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    angle *= TMath::DegToRad();
    Double_t p[7] = {p0, p1, p2, p3, p4, p5, p6};
    //Double_t p[7] = {1.22069, 1405,  3.15,  169, 0.887, 45, 0.354};
    Double_t value =(p[1]*loop(distance,angle,p[2],p[0])+p[3]*loop(distance,angle,p[4],p[0])+p[5]*loop(distance,angle,p[6],p[0]))/(4795.3270);
    //end = clock();
    //cout << "TIME:" << end - begin << endl;
    return value;
}

Double_t NEWF(Double_t x, Double_t y, Double_t par){
    Double_t  r = sqrt(x*x+y*y)+0.1;
    return exp(-par*r)/r;
}
Double_t INT(Double_t x0, Double_t L0, Double_t y, Double_t par){
    //Double_t L0 = (b-a)/2;
    Double_t a = x0 - L0, b = x0 + L0;
    if (a*b<0){
        a *= -1;
        if (b < a) {Double_t ys = b; b = a; a = ys;}
        Double_t s = b-a;
        Double_t result1 = a/18*(NEWF(0,y,par)+NEWF(a,y,par)+4*(NEWF(a/6,y,par)+NEWF(a/2,y,par)+NEWF(5*a/6,y,par))+2*(NEWF(a/3,y,par)+NEWF(2*a/3,y,par)));
        Double_t result2 = s/18*(NEWF(a,y,par)+NEWF(b,y,par)+4*(NEWF(a+s/6,y,par)+NEWF(a+s/2,y,par)+NEWF(a+5*s/6,y,par))+2*(NEWF(a+s/3,y,par)+NEWF(a+2*s/3,y,par)));
        return 2*result1+result2;
    }
    return L0/9*(NEWF(a,y,par)+NEWF(b,y,par)+4*(NEWF(a+L0/3,y,par)+NEWF(a+L0,y,par)+NEWF(a+5*L0/3,y,par))+2*(NEWF(a+2*L0/3,y,par)+NEWF(a+4*L0/3,y,par)));
}
Double_t RES(Double_t distance, Double_t angle, Double_t par, Double_t L0){
    Double_t x0 = distance*cos(angle), y0 = distance*sin(angle);
    Double_t xp = x0+L0 , xm = x0-L0, yp = y0+L0, ym = y0-L0;
    if (ym*yp<0){
            ym *= -1;
            if (yp < ym) {Double_t ys = yp; yp = ym; ym = ys;}
            Double_t s = yp-ym;
            Double_t result1 = ym/18*(INT(x0,L0,0,par)+INT(x0,L0,ym,par)+4*(INT(x0,L0,ym/6,par)+INT(x0,L0,ym/2,par)+INT(x0,L0,5*ym/6,par))+2*(INT(x0,L0,ym/3,par)+INT(x0,L0,2*ym/3,par)));
            Double_t result2 = s/18*(INT(x0,L0,ym,par)+INT(x0,L0,yp,par)+4*(INT(x0,L0,ym+s/6,par)+INT(x0,L0,ym+s/2,par)+INT(x0,L0,ym+5*s/6,par))+2*(INT(x0,L0,ym+s/3,par)+INT(x0,L0,ym+2*s/3,par)));
            return 2*result1+result2;
    }
    return L0/9*(INT(x0,L0,y0-L0,par)+INT(x0,L0,y0+L0,par)+4*(INT(x0,L0,y0-2*L0/3,par)+INT(x0,L0,y0,par)+INT(x0,L0,y0+2*L0/3,par))+2*(INT(x0,L0,y0-L0/3,par)+INT(x0,L0,y0+L0/3,par)));
}
Double_t VALUE(Double_t distance,Double_t angle, Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5, Double_t p6){
    //time_t begin,end;
    //begin = clock();
    if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
    if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    angle *= TMath::DegToRad();
    Double_t p[7] = {p0, p1, p2, p3, p4, p5, p6};
    //Double_t p[7] = {1.22069, 1405,  3.15,  169, 0.887, 45, 0.354};
    Double_t value =(p[1]*RES(distance,angle,p[2],p[0])+p[3]*RES(distance,angle,p[4],p[0])+p[5]*RES(distance,angle,p[6],p[0]))/(3816*0.2*TMath::TwoPi());
    //end = clock();
    //cout << "TIME:" << end - begin << endl;
    return value;
}

Double_t DIGITAL_ARC(Double_t a, Double_t b, Double_t xm, Double_t ci){
    Int_t n = 4;
    Double_t x;
    Double_t value1(0),value2(0);
    //0~0.72*sqrt(xm) n=60;
    Double_t step;
    if ( xm < 0.1 && b < 2.5 ){
        Int_t N = 10;
        Double_t a_cut = 0.72*sqrt(xm);
        x = a;
        step = (a_cut-a)/N;
        for (int i = 1; i < N; i++) value1 += TMath::ACos(xm/(x+i*step))*exp(-ci*(x+i*step));
        value1 = step*((TMath::ACos(xm/a)*exp(-ci*a)+TMath::ACos(xm/a_cut)*exp(-ci*a_cut))/2+value1);
        a = a_cut;
        
        /*a_cut = 1.35*sqrt(xm);;
        x = a_cut;
        step = (b-a_cut)/N;
        for (int i = 1; i < N; i++) value2 += TMath::ACos(xm/(x+i*step))*exp(-ci*(x+i*step));
        value2 = step*((TMath::ACos(xm/a_cut)*exp(-ci*a_cut)+TMath::ACos(xm/b)*exp(-ci*b))/2+value2);
        b = a_cut;*/
        
        /*Int_t N = 20;
        Double_t a_cut = 0.3367*pow(p,0.4);
        x = a;
        step = (a_cut-a)/N;
        for (int i = 1; i < N; i++) value1 += exp(-p/sin(x+i*step));
        value1 = step*(exp(-p/sin(a))+exp(-p/sin(a_cut))/2+value1);
        a = a_cut;
        a_cut = 2.59111*pow(p,0.46);
        x=a_cut;
        step = (b-a_cut)/N;
        for (int i = 1; i < N; i++) value2 += exp(-p/sin(x+i*step));
        value2 = step*(exp(-p/sin(a_cut))+exp(-p/sin(b))/2+value2);
        b = a_cut;*/
    }
    
    x = a;
    Double_t sum1(0),sum2(0);
    step = (b-a)/(2*n);
    for (int i = 1; i < 2*n; i+=2) sum1 += TMath::ACos(xm/(x+i*step))*exp(-ci*(x+i*step));
    for (int i = 2; i < 2*n-1; i+=2) sum2 += TMath::ACos(xm/(x+i*step))*exp(-ci*(x+i*step));
    //if (fabs(a-xm)<0.01) return ((b-a)/6/n)*(TMath::ACos(xm/b)*exp(-ci*b)+4*sum1+2*sum2);else
            return value1+value2+((b-a)/6/n)*(TMath::ACos(xm/a)*exp(-ci*a)+TMath::ACos(xm/b)*exp(-ci*b)+4*sum1+2*sum2);
}
Double_t SRT(Double_t distance, Double_t angle, Double_t ci, Double_t L0){
    int N(0);
    Double_t S[4];
    int w0(-1),w2(-1);
    Double_t x0 = distance*cos(angle), y0 = distance*sin(angle);
    for (int j = 0; j < 2; j++){
        Double_t xp = x0+L0 , xm = x0-L0, yp = y0+L0, ym = y0-L0;
        if (j==0) {
            if (xm>=0 && ym<0) w2 = 1;
            else if (xm<0 && ym<=0) w0 = w2 = 1;
        }
        Double_t x_zd = fabs(xm);
        for (int i=0; i < 2; i++){
            Double_t r_min = sqrt(x_zd*x_zd+ym*ym), r_max = sqrt(x_zd*x_zd+yp*yp);
            Double_t theta_min= TMath::ACos(x_zd/r_min)  ,theta_max= TMath::ACos(x_zd/r_max);
            //if (r_min < 0.01) theta_min = 0;
            if (ym*yp<0)
                S[N] = theta_min/ci*(1-exp(-ci*r_min))+theta_max/ci*(1-exp(-ci*r_max))-2*DIGITAL_ARC(x_zd,r_min,x_zd,ci)-DIGITAL_ARC(r_min,r_max,x_zd,ci);
            else S[N] = (theta_max-theta_min+theta_min*exp(-ci*r_min)-theta_max*exp(-ci*r_max))/ci-DIGITAL_ARC(r_min,r_max,x_zd,ci);
            x_zd = xp;
            N++;
        }
        x0 = distance*sin(angle), y0 = distance*cos(angle);
    }
    return w0*S[0]+S[1]+w2*S[2]+S[3];
}

Double_t MYVALUE(Double_t distance,Double_t angle, Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5, Double_t p6){
    //time_t begin,end;
    //begin = clock();
    if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
    if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
    angle *= TMath::DegToRad();
    Double_t p[7] = {p0, p1, p2, p3, p4, p5, p6};
    //Double_t p[7] = {1.22069, 1405,  3.15,  169, 0.887, 45, 0.354};
    Double_t value =(p[1]*SRT(distance,angle,p[2],p[0])+p[3]*SRT(distance,angle,p[4],p[0])+p[5]*SRT(distance,angle,p[6],p[0]))/(3816*0.2*TMath::TwoPi());
    //end = clock();
    //cout << "TIME:" << end - begin << endl;
    return value;
}


struct INTEGRAL {
    Double_t y, ci, L0;
    void SetPar(Int_t index, Double_t par){
        if (index == 1) L0 = par;
        else if (index == 2) ci = par;
        else if (index == 3) y = par;
        else std::cout << "Parameter error!!" << std::endl;
    }
    Double_t func_Int(Double_t a, Double_t b){
        float value(0);
        if (y < 0.1){
            Int_t N = 3+100*(b-a);
            float step = (b-a)/N, k = a+step, m = b-step/2;
            for (float i = k; i < m; i+=step) value += y*(1-exp(-ci*sqrt(i*i+y*y)))/(i*i+y*y);
            return step*((y*(1-exp(-ci*sqrt(a*a+y*y)))/(a*a+y*y)+y*(1-exp(-ci*sqrt(b*b+y*y)))/(b*b+y*y))/2+value);
        }else{
            Int_t n = 4;
            float step = (b-a)/(2*n), Twostep = 2*step, StepOver2 = step/2;
            float sum1(0), sum2(0), k1 = a + step, k2 = k1 + step, m1 = b - StepOver2, m2 = m1 - step;
            for (float i = k1; i < m1; i += Twostep) sum1 += y*(1-exp(-ci*sqrt(i*i+y*y)))/(i*i+y*y);
            for (float i = k2; i < m2; i += Twostep) sum2 += y*(1-exp(-ci*sqrt(i*i+y*y)))/(i*i+y*y);
            return step/3*(y*(1-exp(-ci*sqrt(a*a+y*y)))/(a*a+y*y)+y*(1-exp(-ci*sqrt(b*b+y*y)))/(b*b+y*y)+4*sum1+2*sum2);
        }
    }
    Double_t line_Int(Double_t x_n, Double_t y_n){
        SetPar(3,y_n);
        if ( x_n-L0 > 0) return func_Int(x_n-L0,x_n+L0);
        else return 2*func_Int(0,L0-x_n)+func_Int(L0-x_n,x_n+L0);
    }
    Double_t block_Int(Double_t distance, Double_t angle, Double_t ci){
        Double_t x0 = distance*cos(angle), y0 = distance*sin(angle);
        Double_t xp = x0+L0 , xm = x0-L0, yp = y0+L0, ym = y0-L0;
        SetPar(2,ci);
        Int_t w1(1),w3(1);
        if (xm>0) { w1 = -1; if (ym>0) w3 = -1; }
        return w1*line_Int(y0,fabs(xm))+line_Int(y0,xp)+w3*line_Int(x0,fabs(ym))+line_Int(x0,yp);
    }
    Double_t shower_Digi(Double_t distance,Double_t angle, Double_t L_1,Double_t L_2, Double_t L_3, Double_t p0, Double_t p1, Double_t p2, Double_t p3, Double_t p4, Double_t p5){
        if ( angle > 90 && angle <= 180 ) angle = 180 - angle;
        else if ( angle > 45 && angle <= 90 ) angle = 90 - angle;
        angle *= TMath::DegToRad();
        Double_t p[6] = {p0, p1, p2, p3, p4, p5};
        SetPar(1,L_1);
        Double_t T1 = block_Int(distance,angle,p[1])/p[1];
        SetPar(1,L_2);
        Double_t T2 = p[2]*block_Int(distance,angle,p[3])/p[3];
        SetPar(1,L_3);
        Double_t T3 = p[4]*block_Int(distance,angle,p[5])/p[5];
        return p[0]*(T1+T2+T3)/3;
    }
}Shower_Function;

int Exec(TString dir_name, TH2D *h, TGraph2D *g2D, Int_t NGamma=1);
int crystal_test_test( TString dir_name="Gamma_one_1G" )
{
    int bin1(100),bin2(100),bin3(100);
    float tx(800),ty(600);
    //double xmin(0),xmax(3.5),ymin(-0.6),ymax(0.6),zmin(0),zmax(0.1);
    double xmin(0),xmax(3.5),ymin(0),ymax(1),zmin(0),zmax(90);
    //double xmin(0),xmax(1),ymin(0),ymax(1),zmin(0),zmax(0.1);
    
    TCanvas* c1=new TCanvas("PANDA1","test1",tx,ty);
    //TCanvas* c2=new TCanvas("PANDA2","test2",tx,ty);
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
    
    TH3D* h3D = new TH3D("Hist2","h2",bin1,xmin,xmax, bin3,zmin,zmax, bin2,ymin,ymax);
    h3D->SetMarkerStyle(7);
    h3D->SetMarkerColorAlpha(kAzure+3, 0.5);
    //h2D->GetYaxis()->SetTitle("E_{ci}");h2D->GetXaxis()->SetTitle("E_{truth}");
    h3D->GetZaxis()->SetTitle("E_{digi}");h3D->GetXaxis()->SetTitle("distance");h3D->GetYaxis()->SetTitle("angle");
    //h1D->GetZaxis()->SetTitle("E");
    h3D->GetXaxis()->CenterTitle();
    h3D->GetYaxis()->CenterTitle();
    h3D->GetZaxis()->CenterTitle();
    
    TH2D* h2D = new TH2D("Hist1","h1",bin1,xmin,xmax,bin2,ymin,ymax);
    //TH2D* h2D = new TH2D("Hist1","h1",10,xmin,xmax,10,zmin,zmax);
    h2D->SetMarkerStyle(22);
    h2D->SetMarkerColorAlpha(kAzure+3, 0.5);
    //h2D->GetYaxis()->SetTitle("E_{ci}");h2D->GetXaxis()->SetTitle("E_{truth}");
    h2D->GetYaxis()->SetTitle("E_{digi}");h2D->GetXaxis()->SetTitle("distance");
    //h1D->GetZaxis()->SetTitle("E");
    h2D->GetXaxis()->CenterTitle();
    h2D->GetYaxis()->CenterTitle();
    h2D->GetZaxis()->CenterTitle();
    
    TGraph2D* g2D = new TGraph2D();
    g2D->SetMarkerStyle(7);
    g2D->SetMarkerColorAlpha(kAzure+3, 0.5);
    g2D->GetZaxis()->SetTitle("E_{digi}");g2D->GetXaxis()->SetTitle("distance");g2D->GetYaxis()->SetTitle("angle");
    g2D->GetXaxis()->CenterTitle();
    g2D->GetYaxis()->CenterTitle();
    g2D->GetZaxis()->CenterTitle();
    
    if( Exec(dir_name, h2D, g2D, 1) ) return 1;
    /*int N(0);
    Double_t x(0), y(0);
    for (int i = 0; i < 100; i++){
        y = 0;
        for (int j = 0; j < 100; j++){
            //g2D->SetPoint(N,x,y,mf(x,y,1.064, 1404.71, 3.15506, 169.204, 0.887089, 45.4251, 0.354403));
            g2D->SetPoint(N,x,y,result(x,y,1.064, 1404.71, 3.15506, 169.204, 0.887089, 45.4251, 0.354403));
            y += 0.9;
            N++;
        }
        x += 0.035;
    }*/

    c1->cd();
    //c1->SetGridy();
    //h3D->Draw("SCAT");
    //h2D->Draw("SCAT");
    //c2->cd();
    g2D->Draw("p.");
    
    TF2* f2=new TF2("f2","Shower_Function.shower_Digi(x,y,[0],[1],[2],  [3],  [4], [5],[6], [7],[8])",0,8,0,90);
    f2->SetParameters( 1.22, 1.22, 1.22,   0.3,   3.15,  0.12,0.887,   0.032,0.354);
    //f2->SetParLimits(0, 1.0, 2.0);
    //f2->SetParLimits(1, 2,5);
    //f2->SetParLimits(2, 0.5, 1.5);
    
    
    //TF2* f2=new TF2("f2","[6]*Shower_Function.shower_Digi(x,y,[0],[1],[2],[3],[4],[5])",0,14,0,90);
    //TF2* f2=new TF2("f2","[1]*mf(x,y,[0],1405,3.2,170,0.89,45,0.35)",0,3,0,90);
    //TF1* f2=new TF1("f2","mf(x,0,[0],[1],[2],[3])",0,3.5);
    //TF1* f2=new TF1("f2","mf(1.3,x,[0],[1],[2],[3])",0,90);
    //f2->SetParameters(1.064, 1.0);
    //f2->SetParLimits(0, 1.0, 2.0);
    //f2->SetParLimits(1, 0.8,1.1);
    //f2->SetParameters( 1.22069, 3.2,  0.12, 0.89, 0.032, 0.354403, 1);
    //f2->SetParameters( 37147,  168.255,  -15302.4, 44.5161, 1489.1, 1.74012);
    //f2->SetParLimits(0, 1.0, 2.0);
    //f2->SetParLimits(1, 1000,2000);
    //f2->SetParLimits(2, 2.9, 3.5);
    //f2->SetParLimits(3, 150, 200);
    //f2->SetParLimits(4, 0.7, 1.2);
    //f2->SetParLimits(5, 20, 70);
    //f2->SetParLimits(6, 0.1, 0.5);
    //f2->SetParLimits(4, 0.5, 1.5);
    //f2->Draw();
    //h3D->Fit(f2,"R");
    g2D->Fit(f2,"R");
    //f2->Draw();
    
    TLegend * leg1 = new TLegend(0.61,0.72,0.88,0.85);
    leg1->AddEntry(h2D, "Crystal calculated", "P");
    leg1->Draw("SAME");
    
    return 0;
}

//*******************************************************************************************************//
int Exec(TString dir_name, TH2D *h, TGraph2D* g2D, Int_t NGamma){
    //NGamma: Number of photons produced
    
    TString file_path_sim = "../data/"+dir_name+"/evtcomplete_sim.root";
    TString file_path_digi = "../data/"+dir_name+"/evtcomplete_digi.root";
    
    FairRunAna *fRun = new FairRunAna();
    TFile* file = new TFile(file_path_sim);
    FairFileSource* source = new FairFileSource(file,"InputFile");
    FairRootManager* ioman = FairRootManager::Instance();
    ioman->SetSource(source);
    ioman->InitSource();
    
    TClonesArray* fMCTrackArray = (TClonesArray*) ioman->GetObject("MCTrack");
    if (!fMCTrackArray) return -1;
    TClonesArray* fHitArray = (TClonesArray*) ioman->GetObject("EmcHit");
    if (!fHitArray) return -1;
    
    TFile* f = new TFile(file_path_digi);
    TTree* t = (TTree*)f->Get("pndsim");
    TClonesArray* fBumpArray = new TClonesArray("PndEmcBump");
    t->SetBranchAddress("EmcBump",&fBumpArray);
    TClonesArray* fDigiArray = new TClonesArray("PndEmcDigi");
    t->SetBranchAddress("EmcDigi",&fDigiArray);
    if (!fBumpArray) return -1;
    TClonesArray* fClusterArray = new TClonesArray("PndEmcCluster");
    t->SetBranchAddress("EmcCluster",&fClusterArray);
    if (!fClusterArray) return -1;
    
    int N(0);
    Int_t maxEvtNo = t->GetEntries();
    maxEvtNo /= 10;
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
        ioman->ReadEvent(ievt); // read event by event
        t->GetEntry(ievt);
        if (maxEvtNo>=100 && ievt%(maxEvtNo/100)==0) cout << 100 * (int)ievt/maxEvtNo << "%" << endl;
        int nhits = fHitArray->GetEntriesFast();
        int nclusters = fClusterArray->GetEntriesFast();
        int nbumps = fBumpArray->GetEntriesFast();
        int ndigis = fDigiArray->GetEntriesFast();
        
        //Get the momentum of each photon
        std::vector<TVector3> Gamma_mom;
        for (int iGamma = 0; iGamma < NGamma; iGamma++) {
            PndMCTrack *mcTrack = (PndMCTrack *)fMCTrackArray->At(iGamma);
            Gamma_mom.push_back(mcTrack->GetMomentum());
        }
        
        //Calculate the average distance between photons
        Double_t distance(0);
        Int_t Ncunt(0);
        for (int iGamma = 0; iGamma < NGamma-1; iGamma++) {
            for (int jGamma = iGamma+1; jGamma < NGamma; jGamma++) {
                Double_t TheDistance = ((65.0/Gamma_mom[iGamma].Pt())*Gamma_mom[iGamma]-(65.0/Gamma_mom[jGamma].Pt())*Gamma_mom[jGamma]).Mag();
                distance += TheDistance;
                Ncunt++;
            }
        }
        distance /= Ncunt;
        
        //Exclude events generated electron-positron
        std::map<Int_t, bool> Exist;
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            std::set<FairLink> links = (hit->GetTrackEntering()).GetLinks();
            for (std::set<FairLink>::iterator linkIter = links.begin(); linkIter != links.end(); linkIter++) {
                for (int iGamma = 0; iGamma < NGamma; iGamma++)
                    if (linkIter->GetIndex() == iGamma) Exist[iGamma] = true;
            }
        }
        if (Exist.size() != NGamma) continue;
        
        //Get the true energy of each shower
        std::map<Int_t, Double_t> truth_E;
        for (int iGamma = 0; iGamma < NGamma; iGamma++) truth_E[iGamma] = 0.0;
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            std::map<Int_t, Double_t>  dep = hit->GetDepositedEnergyMap();
            std::map<Int_t, Double_t>::iterator ptr;
            for ( ptr = dep.begin(); ptr != dep.end(); ptr++) {
                for (int iGamma = 0; iGamma < NGamma; iGamma++)
                    if (ptr->first == iGamma) truth_E[iGamma] += ptr->second;
            }
        }
        
        //Match bump for each photon
        std::vector<Int_t> match;
        for (int iGamma = 0; iGamma < NGamma; iGamma++) {
            Double_t min_d(99999);
            Int_t index(-1);
            for (int i = 0; i < nbumps; i++) {
                PndEmcBump* Bump = (PndEmcBump*)fBumpArray->At(i);
                TVector3 pos = Bump->position();
                Double_t d = pos.Mag()*sin(Gamma_mom[iGamma].Angle(pos));
                if (d < min_d) { min_d = d; index = i; }
            }
            if ( index == -1 ) return 1;
            match.push_back(index);
        }
        
        PndEmcBump* Bump = (PndEmcBump*)fBumpArray->At(0);
        PndEmcCluster *theCluster = (PndEmcCluster *)fClusterArray->At(0);
        std::map<Int_t, Int_t> theMaximaDigis = theCluster->LocalMaxMap();
        
        std::map<Int_t, Int_t>::iterator p;
        Int_t digi_seed(-1),digi_seed_id(-1);
        int c(0);
        for (p = theMaximaDigis.begin(); p != theMaximaDigis.end(); p++){
            digi_seed = p->second;
            digi_seed_id = p->first;
            c++;
        }
        if ( c!=1 ) continue;
        //cout <<  "c:" << c << endl;
        
        PndEmcDigi* digi = (PndEmcDigi*)fDigiArray->At(digi_seed);
        double Seed_Energy = digi->GetEnergy();
        TVector3 Seed_pos = digi->where();
        TVector3 Cent_pos = Bump->where();
        //TVector3 Cent_pos = (65.0/Gamma_mom[0].Pt())*Gamma_mom[0];
        
        for (int i = 0; i < nhits; i++) {
            PndEmcHit* hit = (PndEmcHit*)fHitArray->At(i);
            //if (hit->GetDetectorID() == digi_seed_id) continue;
            //if (hit->GetDetectorID() != digi_seed_id) continue;
            //TVector3 Det_Pos(hit->GetX(), hit->GetY(), (hit->GetZ()));
            TVector3 Det_Pos;
            double Truth_Energy = hit->GetEnergy();
            double Digi_Energy = -1;
            for (int j = 0 ; j < ndigis ; j++){
                PndEmcDigi* idigi = (PndEmcDigi*)fDigiArray->At(j);
                if (idigi->GetDetectorId() == hit->GetDetectorID()) {
                Digi_Energy = idigi->GetEnergy();
                Det_Pos = idigi->where();
                }
            }
            if (Digi_Energy == -1) continue;
            double Eci = Seed_Energy * rat(&Det_Pos, &Seed_pos, &Cent_pos, 1.25);
            if (hit->GetDetectorID() == digi_seed_id) Eci = Seed_Energy;
            double Distance = DD(&Det_Pos, &Cent_pos, 1.25);
            double angle = AA(&Det_Pos, &Cent_pos, 1.25);
            //if ((Eci - Truth_Energy) < 0.02) continue;
            //h->Fill(Distance,Eci - Truth_Energy);
            //h->Fill(Distance,Eci - Digi_Energy);
            //h->Fill(Distance,Digi_Energy);
            //h->Fill(Distance,angle,Digi_Energy);
            //h3D->Fill(Distance,angle,Digi_Energy);
            //h3D->Fill(Distance,angle,mf(Distance,angle)/7);
            //h->Fill(Truth_Energy,Eci);
            //h->Fill(Eci,Eci - Truth_Energy);
            /*double dtheta = Det_Pos.Theta();
            double dphi = Det_Pos.Phi();
            double ctheta = Cent_po s.Theta();
            double cphi = Cent_pos.Phi();
            dtheta *= TMath::RadToDeg();
            dphi *= TMath::RadToDeg();
            ctheta *= TMath::RadToDeg();
            cphi *= TMath::RadToDeg();
            double Distance = sqrt((dtheta-ctheta)*(dtheta-ctheta)+(dphi-cphi)*(dphi-cphi));
            cout << dtheta << endl;
            double angle = TMath::ATan(fabs((dphi-cphi)/(dtheta-ctheta)));*/
            if (Distance > 8 || angle<0 || angle>90 ) continue;
            g2D->SetPoint(N,Distance,angle,Digi_Energy);
            N++;
            //if (Digi_Energy >= 0) h->Fill(Eci - Digi_Energy);
        }
        //cout <<  "c:" << c << endl;
        //N++;
    }
    cout << "Max Event Nomber:" << maxEvtNo << ", " << "Passed:" << N << endl;
    return 0;
}
