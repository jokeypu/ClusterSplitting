int Exec(const TString my_dir_name);

int pi0_mass_pre()
{
    FairRunAna *fRun= new FairRunAna();
    if (Exec("pi0_test_2.0GeV")) return 1;
    if (Exec("pi0_test_2.5GeV")) return 1;
    if (Exec("pi0_test_3.0GeV")) return 1;
    if (Exec("pi0_test_3.5GeV")) return 1;
    if (Exec("pi0_test_4.0GeV")) return 1;
    if (Exec("pi0_test_4.5GeV")) return 1;
    if (Exec("pi0_test_5.0GeV")) return 1;
    if (Exec("pi0_test_5.5GeV")) return 1;
    if (Exec("pi0_test_6.0GeV")) return 1;

    return 0;
}

int Exec(const TString my_dir_name){

    cout << "-INFO  " << my_dir_name << endl;
    TString data_dir = gSystem->Getenv("PND_DATA_DIR");

    TString my_file_sim = data_dir + "/" + my_dir_name  + "/sim/sim_emc_*";
    TChain *chain_sim = new TChain("pndsim");
    chain_sim->Add(my_file_sim);

    TString my_file_raw = data_dir + "/" + my_dir_name  + "/reco_raw/reco_emc_*";
    TChain *chain_raw = new TChain("pndsim");
    chain_raw->Add(my_file_raw);

    TString my_file_new = data_dir + "/" + my_dir_name  + "/reco_new/reco_emc_*";
    TChain *chain_new = new TChain("pndsim");
    chain_new->Add(my_file_new);

    TFile f("doc/pi0_mass.root","update");
    TTree mytree(my_dir_name, "pi0_mass");
    Double_t Mass_raw, Mass_new, theta;
    mytree.Branch("Mass_raw",&Mass_raw,"Mass_raw/D");
    mytree.Branch("Mass_new",&Mass_new,"Mass_new/D");
    mytree.Branch("theta",&theta,"theta/D");

    TClonesArray* fMCTrackArray = new TClonesArray("PndMCTrack");
    chain_sim->SetBranchAddress("MCTrack",&fMCTrackArray);
    if (!fMCTrackArray) return -1;

    TClonesArray* fClusterArray_raw = new TClonesArray("PndEmcCluster");
    chain_raw->SetBranchAddress("EmcCluster",&fClusterArray_raw);
    if (!fClusterArray_raw) return -1;

    TClonesArray* fBumpArray_raw = new TClonesArray("PndEmcBump");
    chain_raw->SetBranchAddress("EmcBump",&fBumpArray_raw);
    if (!fBumpArray_raw) return -1;

    TClonesArray* fClusterArray_new = new TClonesArray("PndEmcCluster");
    chain_new->SetBranchAddress("EmcCluster",&fClusterArray_new);
    if (!fClusterArray_new) return -1;

    TClonesArray* fBumpArray_new = new TClonesArray("PndEmcBump");
    chain_new->SetBranchAddress("EmcBump",&fBumpArray_new);
    if (!fBumpArray_new) return -1;
   
    Int_t maxEvtNo = chain_sim->GetEntries();
    for (Int_t ievt = 0; ievt < maxEvtNo; ievt++) {
	if (maxEvtNo>=100 && ievt%(maxEvtNo/100)==0) cout << 100 * (int)ievt/maxEvtNo << "%" << endl;
        chain_sim->GetEntry(ievt);
        chain_raw->GetEntry(ievt);
        chain_new->GetEntry(ievt);
        int nclusters_raw = fClusterArray_raw->GetEntriesFast();
        int nbumps_raw = fBumpArray_raw->GetEntriesFast();
        int nclusters_new = fClusterArray_new->GetEntriesFast();
        int nbumps_new = fBumpArray_new->GetEntriesFast();

 	if (nclusters_raw != 1 && nclusters_raw != 2) continue;       
 	if (nclusters_new != 1 && nclusters_new != 2) continue;       
 	if (nbumps_raw != 2 || nbumps_new != 2) continue;       
	
        PndMCTrack *mcTrack = (PndMCTrack *)fMCTrackArray->At(0);
        TVector3 mom = mcTrack->GetMomentum();
        theta = mom.Theta();

        std::vector <TLorentzVector*> All_P_raw;
        std::vector <TLorentzVector*> All_P_new;
        All_P_raw.clear();
        All_P_new.clear();

        for (unsigned ibump = 0; ibump < nbumps_raw ; ibump++){
            PndEmcBump* bump = (PndEmcBump*)fBumpArray_raw->At(ibump);
            //Double_t bump_e = bump->energy();
            Double_t bump_e = bump->GetEnergy();
            TVector3 p = bump->where();
            p.SetMag(bump_e);
            All_P_raw.push_back(new TLorentzVector(p, bump_e));
        }
        TLorentzVector P_pi0_raw = (*All_P_raw[0]) + (*All_P_raw[1]);
        delete All_P_raw[0]; delete All_P_raw[1];
        Mass_raw = 1000 * P_pi0_raw.M();  //MeV

        for (unsigned ibump = 0; ibump < nbumps_new ; ibump++){
            PndEmcBump* bump = (PndEmcBump*)fBumpArray_new->At(ibump);
            //Double_t bump_e = bump->energy();
            Double_t bump_e = bump->GetEnergy();
            TVector3 p = bump->where();
            p.SetMag(bump_e);
            All_P_new.push_back(new TLorentzVector(p, bump_e));
        }
        TLorentzVector P_pi0_new = (*All_P_new[0]) + (*All_P_new[1]);
        delete All_P_new[0]; delete All_P_new[1];
        Mass_new = 1000 * P_pi0_new.M();  //MeV

	mytree.Fill();
    }

    mytree.Write();
    delete chain_sim;
    delete chain_raw;
    delete chain_new;
    delete fMCTrackArray;
    delete fClusterArray_raw;
    delete fClusterArray_new;
    delete fBumpArray_raw;
    delete fBumpArray_new;
    f.Close();
    return 0;
}
