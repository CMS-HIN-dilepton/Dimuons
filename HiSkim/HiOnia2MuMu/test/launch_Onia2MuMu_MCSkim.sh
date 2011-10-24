#!/bin/bash                                          
                                                     
function command(){                                  
     echo ">>>" $@                                   
     eval $@                                         
}

workDir=$PWD  
                                                                       
logDir=${workDir}"/BATCHJOBS/MC_v7h0/"
command "mkdir -p $logDir"

castorDirIn="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/MC/v7/"
castorDirOut="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/MC/v7/Skims/h0/"
command "rfmkdir -p $castorDirOut"


j=0
jobNb=""

#for file in `nsls $castorDirIn`
#for i in `seq 0 99`
for fileList in `ls inputFilesMC_10?`
do
    echo $fileList;
    # file="MC_PromptJpsi_FEVTDEBUGHLT_"${i}".root"
    jobNb="10"${j};
    let j=${j}+1;
    name="onia2MuMuPAT_MC_${jobNb}"
    outfilename="${name}.root"


#Start to write the script
    cat > job_Onia2MuMu_MCSkim_${jobNb}.sh << EOF
#!/bin/bash

function command(){                                  
     echo ">>>" \$@                                   
     eval \$@                                         
}


workDir="/afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/"
logDir=${workDir}"/BATCHJOBS/MC_v2h0/"
castorDirIn="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/MC/v2/"
castorDirOut="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/MC/v2/Skims/h0/"

cd /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/
eval \`scramv1 runtime -sh\`
cd -
cp /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/onia2MuMuPATHIMC_CAF_cfg.py .
cp /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/${fileList} .
echo "before running cmsRun"
ls -l

command "cmsRun onia2MuMuPATHIMC_CAF_cfg.py inputFiles_clear inputFiles_load=${fileList} print 2>&1 | tee -a $logDir${name}.log"

echo "after running cmsRun"
ls -l

command "rfcp onia2MuMuPAT_MC.root ${castorDirOut}${outfilename}"
command "rfcp tnpSta_MC.root ${castorDirOut}tnpSta_MC_${jobNb}.root"
command "rfcp tnpMuID_MC.root ${castorDirOut}tnpMuID_MC_${jobNb}.root"
command "rfcp tnpTrig_MC.root ${castorDirOut}tnpTrig_MC_${jobNb}.root"

command "gzip -f $logDir${name}.log"         

EOF

	chmod 755 job_Onia2MuMu_MCSkim_${jobNb}.sh
	
	bsub -q 1nd -J $name /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/job_Onia2MuMu_MCSkim_${jobNb}.sh

done
