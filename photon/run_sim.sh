. $PND_CONFIG_0
export PATH=$PATH:/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin
if [ ! -n "${1}" ] ;then
   echo "ERROR: Missing input parameters."
   echo "usage :   source run_sim.sh {energy}"
   return
fi

for((i=0;i<15;i++))
do

hep_pnd sim_photon.txt -argu ${1} $i

done
