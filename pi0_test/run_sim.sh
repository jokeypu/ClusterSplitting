. $PND_CONFIG_1
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin
if [ ! -n "${1}" ] ;then
   echo "ERROR: Missing input parameters."
   echo "usage :   source run_sim.sh {energy}"
   return
fi

for((i=5;i<10;i++))
do

hep_pnd sim_pi0.txt -argu ${1} $i

done
