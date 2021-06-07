. $PND_CONFIG_1
if [ -d $MY_RUN_DIR/results/doc ];then
        if [ ! -d $MY_RUN_DIR/results/doc/step1_di-photon_picture ];then
                mkdir $MY_RUN_DIR/results/doc/step1_di-photon_picture
        fi
        if [ ! -d $MY_RUN_DIR/results/doc/step2_di-photon_picture ];then
                mkdir $MY_RUN_DIR/results/doc/step2_di-photon_picture
        fi
        if [ ! -d $MY_RUN_DIR/results/doc/step1_pi0_picture ];then
                mkdir $MY_RUN_DIR/results/doc/step1_pi0_picture
        fi
        if [ ! -d $MY_RUN_DIR/results/doc/step2_pi0_picture ];then
                mkdir $MY_RUN_DIR/results/doc/step2_pi0_picture
        fi
else
mkdir $MY_RUN_DIR/results/doc
mkdir $MY_RUN_DIR/results/doc/step1_di-photon_picture
mkdir $MY_RUN_DIR/results/doc/step2_di-photon_picture
mkdir $MY_RUN_DIR/results/doc/step1_pi0_picture
mkdir $MY_RUN_DIR/results/doc/step2_pi0_picture
fi     
