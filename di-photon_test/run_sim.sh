. $PND_CONFIG_1
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin
if [ ! -n "${1}" ] ;then
   echo "ERROR: Missing input parameters."
   echo "usage :   source run_sim.sh {energy}"
   return
fi

for((i=0;i<500;i++))
do

null_hep_pnd sim_di-photon.txt -argu ${1} $i 3
null_hep_pnd sim_di-photon.txt -argu ${1} $i 15
null_hep_pnd sim_di-photon.txt -argu ${1} $i 20

done
