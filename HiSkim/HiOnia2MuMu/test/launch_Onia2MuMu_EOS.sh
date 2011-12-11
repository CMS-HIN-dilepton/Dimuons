#!/bin/bash                                          
                                                     
function command(){                                  
     echo ">>>" $@                                   
     eval $@                                         
}

workDir=$PWD  
                                                                       
logDir=${workDir}"/BATCHJOBS/v1h0/"
command "mkdir -p $logDir"

castorDirOut="/store/caf/user/tdahms/HeavyIons/Data2011/pp/v1/Skims/ReReco/"
#command "cmsMkdir -p $castorDirOut"


j=0
jobNb=""

for fileList in `ls ppFiles_??`
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


workDir=$PWD  
logDir="${logDir}"
castorDirOut="${castorDirOut}"

cd /afs/cern.ch/user/t/tdahms/scratch0/devel/CMSSW_4_4_2_patch5/src/
eval \`scramv1 runtime -sh\`
source /afs/cern.ch/cms/caf/setup.sh
cd -
cp /afs/cern.ch/user/t/tdahms/scratch0/devel/CMSSW_4_4_2_patch5/src/HiSkim/HiOnia2MuMu/test/onia2MuMuPAT_pp_lowMass_cfg.py .
cp /afs/cern.ch/user/t/tdahms/scratch0/devel/CMSSW_4_4_2_patch5/src/HiSkim/HiOnia2MuMu/test/${fileList} .
echo "before running cmsRun"
ls -l

command "cmsRun onia2MuMuPAT_pp_lowMass_cfg.py inputFiles_clear inputFiles_load=${fileList} print 2>&1 | tee -a $logDir${name}.log"

echo "after running cmsRun"
ls -l

command "cmsStage onia2MuMuPAT_lowmass.root ${castorDirOut}${outfilename}"

command "gzip -f $logDir${name}.log"

EOF

	chmod 755 job_Onia2MuMu_EOS_${jobNb}.sh
	
	bsub -q 1nd -J $name /afs/cern.ch/user/t/tdahms/scratch0/devel/CMSSW_4_4_2_patch5/src/HiSkim/HiOnia2MuMu/test/job_Onia2MuMu_EOS_${jobNb}.sh

done
