int Exec(const TString my_dir_name, Int_t my_index);
int di_photon_energy_pre(TString str_angle = "A15", Int_t angle_index = 15)
{
    FairRunAna *fRun= new FairRunAna();
    if (Exec(str_angle, "di-photon_test_0.5GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_1.0GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_1.5GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_2.0GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_2.5GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_3.0GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_3.5GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_4.0GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_4.5GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_5.0GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_5.5GeV", angle_index)) return 1;
    if (Exec(str_angle, "di-photon_test_6.0GeV", angle_index)) return 1;
    return 0;
}


int Exec(TString add_str, TString raw_dir_name, Int_t my_index){

    TString my_dir_name = add_str + "_" + raw_dir_name;
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

    TFile f("doc/di-photon_energy.root","update");
    TTree mytree(my_dir_name, "di-photon_energy");
    Double_t Energy_raw, Energy_new;
    Int_t theta_index;
    mytree.Branch("Energy_raw",&Energy_raw,"Energy_raw/D");
    mytree.Branch("Energy_new",&Energy_new,"Energy_new/D");
    mytree.Branch("theta",&theta_index,"theta/I");

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

 	if (nclusters_raw != 1 || nbumps_raw != 2) continue;       
 	if (nclusters_new != 1 || nbumps_new != 2) continue;       
	
        theta_index = my_index;

	PndEmcBump* Bump_raw = (PndEmcBump*)fBumpArray_raw->At(0);
        //Energy_raw = Bump->energy();
        Energy_raw = Bump_raw->GetEnergy();

	PndEmcBump* Bump_new = (PndEmcBump*)fBumpArray_new->At(0);
        //Energy_new = Bump->energy();
        Energy_new = Bump_new->GetEnergy();

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
