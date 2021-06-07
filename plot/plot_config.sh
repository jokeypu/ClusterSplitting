export MY_PAR_FILE="$MY_RUN_DIR/simparams.root"
export MY_ASCII_FILE="$MY_RUN_DIR/my_emc.par"
if [ -n "ls $MY_RUN_DIR/plot/core*" ];then
        rm -rf $MY_RUN_DIR/plot/core*
fi
if [ -d $MY_RUN_DIR/plot/doc ];then
	if [ ! -d $MY_RUN_DIR/plot/doc/step1_picture ];then
		mkdir $MY_RUN_DIR/plot/doc/step1_picture
	fi
	if [ ! -d $MY_RUN_DIR/plot/doc/step2_picture ];then
		mkdir $MY_RUN_DIR/plot/doc/step2_picture
	fi
	if [ ! -d $MY_RUN_DIR/plot/doc/step3_picture ];then
		mkdir $MY_RUN_DIR/plot/doc/step3_picture
	fi
else 
mkdir $MY_RUN_DIR/plot/doc
mkdir $MY_RUN_DIR/plot/doc/step1_picture
mkdir $MY_RUN_DIR/plot/doc/step2_picture
mkdir $MY_RUN_DIR/plot/doc/step3_picture
fi
