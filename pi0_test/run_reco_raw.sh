Add_string="raw"
. $PND_CONFIG_1
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin
if [ ! -n "${1}" ];then
   echo "ERROR: Missing input parameters."
   echo "usage :   source run_reco_raw.sh {energy}"
   return
fi

for((i=5;i<10;i++))
do

hep_pnd reco_pi0.txt -argu ${1} $i $Add_string

done
unset Add_string
