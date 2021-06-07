// Macro for running Panda digitization tasks
// to run the macro:
// root  digi_complete.C  or in root session root>.x  digi_complete.C
int reco_emc(Int_t nEvents = 0)
{
  //-----User Settings:------------------------------------------------------
  TString emcInputFile    = gSystem->Getenv("MY_INPUT_FILE");
  TString emcOutputFile   = gSystem->Getenv("MY_OUTPUT_FILE");
  TString emcParFile      = gSystem->Getenv("MY_PAR_FILE");
  TString emcAsciiFile    = gSystem->Getenv("MY_ASCII_FILE");
  
  // -----   Initial Settings   --------------------------------------------
  //PndMasterRunAna *fRun= new PndMasterRunAna();
  //fRun->SetInput(input);
  //fRun->SetOutput(output);
  //fRun->SetParamAsciiFile(parAsciiFile);
  //fRun->Setup(prefix);

  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile(emcInputFile);
  fRun->SetOutputFile(emcOutputFile);


  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(emcParFile.Data());

  FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
  parIo1->open(emcAsciiFile.Data(),"in");

  rtdb->setFirstInput(parInput1);
  rtdb->setSecondInput(parIo1);

  PndPersistencyTask *task;

  // -----   Add tasks   ----------------------------------------------------

  task = new PndEmcHitsToWaveform();
  task->SetPersistency(kFALSE);
  task->SetVerbose(0);
  fRun->AddTask(task);

  task = new PndEmcWaveformToDigi();
  task->SetPersistency(kTRUE);
  task->SetVerbose(0);
  fRun->AddTask(task);

  task = new PndEmcMakeCluster();
  task->SetVerbose(0);
  fRun->AddTask(task);

  task = new PndEmcMakeBump();
  task->SetVerbose(0);
  fRun->AddTask(task);

  // -----   Intialise and run   --------------------------------------------
  fRun->Init();
  fRun->Run(0, nEvents);
  return 0;
}
