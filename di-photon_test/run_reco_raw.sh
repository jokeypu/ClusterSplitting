Add_string="raw"
. $PND_CONFIG_1
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin
if [ ! -n "${1}" ];then
   echo "ERROR: Missing input parameters."
   echo "usage :   source run_reco_raw.sh {energy}"
   return
fi

for((i=0;i<500;i++))
do

null_hep_pnd reco_di-photon.txt -argu ${1} $i $Add_string 3
null_hep_pnd reco_di-photon.txt -argu ${1} $i $Add_string 15
null_hep_pnd reco_di-photon.txt -argu ${1} $i $Add_string 20

done
unset Add_string
