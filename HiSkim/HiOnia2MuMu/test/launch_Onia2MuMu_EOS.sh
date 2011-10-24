#!/bin/bash                                          
                                                     
function command(){                                  
     echo ">>>" $@                                   
     eval $@                                         
}

workDir=$PWD  
                                                                       
logDir=${workDir}"/BATCHJOBS/v12h0/"
command "mkdir -p $logDir"

castorDirOut="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/Data2010/v12/Skims/ReReco/"
command "rfmkdir -p $castorDirOut"


j=0
jobNb=""

for fileList in `ls inFiles???`
do
    echo $fileList;
    # file="MC_PromptJpsi_FEVTDEBUGHLT_"${i}".root"
    jobNb=${j};
    let j=${j}+1;
    name="onia2MuMuPAT_${jobNb}"
    outfilename="${name}.root"


#Start to write the script
    cat > job_Onia2MuMu_EOS_${jobNb}.sh << EOF
#!/bin/bash

function command(){                                  
     echo ">>>" \$@                                   
     eval \$@                                         
}


workDir="/afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/"
logDir=${workDir}"/BATCHJOBS/v12h0/"
castorDirOut="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/Data2010/v12/Skims/ReReco/"

cd /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/
eval \`scramv1 runtime -sh\`
source /afs/cern.ch/cms/caf/eos.sh
cd -
cp /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/onia2MuMuPATHIData_EOS_cfg.py .
cp /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/${fileList} .
echo "before running cmsRun"
ls -l

command "cmsRun onia2MuMuPATHIData_EOS_cfg.py inputFiles_clear inputFiles_load=${fileList} print 2>&1 | tee -a $logDir${name}.log"

echo "after running cmsRun"
ls -l

command "rfcp onia2MuMuPAT.root ${castorDirOut}${outfilename}"
command "rfcp tnpSta.root ${castorDirOut}tnpSta_${jobNb}.root"
command "rfcp tnpMuID.root ${castorDirOut}tnpMuID_${jobNb}.root"
command "rfcp tnpTrig.root ${castorDirOut}tnpTrig_${jobNb}.root"

command "rfcp MinBiasCentralityEarly_Histo.root ${castorDirOut}MinBiasCentralityEarly_Histo_${jobNb}.root"
command "rfcp MinBiasCentrality_Histo.root ${castorDirOut}MinBiasCentrality_Histo_${jobNb}.root"
command "rfcp DoubleMuOpenCentrality_Histo.root ${castorDirOut}DoubleMuOpenCentrality_Histo_${jobNb}.root"

command "gzip -f $logDir${name}.log"

EOF

	chmod 755 job_Onia2MuMu_EOS_${jobNb}.sh
	
	bsub -q 1nd -J $name /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_9_patch1/src/HeavyFlavorAnalysis/Onia2MuMu/test/job_Onia2MuMu_EOS_${jobNb}.sh

done
