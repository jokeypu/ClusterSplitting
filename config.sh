cd $(dirname $BASH_SOURCE)
export MY_RUN_DIR=`echo $(pwd -LP)`

export PND_DATA_DIR="/scratchfs/bes/$USER/pnd_data"   # 设置数据存储路径
#export PND_DATA_DIR="/scratchfs/bes/zhangziyu182/pnd_data"   # 设置数据存储路径

alias hep_pnd='hep_sub -g panda -o $MY_RUN_DIR/info/ -e $MY_RUN_DIR/info/ '  # 提交作业快捷命令 hep_pnd [jobscript]
alias null_hep_pnd='hep_sub -g panda -o /dev/null -e /dev/null '  # 提交作业快捷命令(不产生输出文本) null_hep_pnd [jobscript]

export PND_CONFIG_0="/cvmfs/panda.ihep.ac.cn/PandaRoot/build_oct19/config.sh"
export PND_CONFIG_1="/besfs5/users/puqing/Panda/PandaRoot_Raw/build/config.sh"  #Raw
export PND_CONFIG_2="/besfs5/users/puqing/Panda/PandaRoot_New/build/config.sh"  #New

export SIMPATH=/cvmfs/panda.ihep.ac.cn/FairSoft/build_jun19p1 # FairSoft
export FAIRROOTPATH=/cvmfs/panda.ihep.ac.cn/FairRoot/install_v18.2.0 # FairRoot
. $PND_CONFIG_0 > /dev/null # 建立PandaRoot环境

export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin # Panda提交作业环境变量

. plot/plot_config.sh #配置plot
. results/config_results.sh #配置results
if [ ! -d $PND_DATA_DIR ];then
	mkdir $PND_DATA_DIR 
fi

if [ ! -f $MY_RUN_DIR/.log_time ];then
	echo "0" > $MY_RUN_DIR/.log_time
fi
TIME_LAST=`cat $MY_RUN_DIR/.log_time`
TIME_NOW=`echo  $(ac) | tr -cd "[0-9]"`
if [ ` expr $TIME_NOW - $TIME_LAST ` -ge 3000 ]||[ ` expr $TIME_NOW - $TIME_LAST ` -le 0 ];then  # 30分钟登陆时长后自动清理info内容
	if [ ! -d $MY_RUN_DIR/info/ ];then
        	mkdir $MY_RUN_DIR/info
	else if [ ! "`ls -A $MY_RUN_DIR/info/`" = "" ];then
        	rm -rf $MY_RUN_DIR/info/*
     	     fi
	fi
echo $TIME_NOW > $MY_RUN_DIR/.log_time
fi
unset TIME_LAST
unset TIME_NOW

if [ -n "ls $MY_RUN_DIR/core*" ];then
	rm -rf $MY_RUN_DIR/core*
fi

cd - > /dev/null
