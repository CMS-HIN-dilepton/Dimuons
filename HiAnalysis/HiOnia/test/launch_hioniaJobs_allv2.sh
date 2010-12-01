#!/bin/bash                                          
                                                     
function command(){                                  
     echo ">>>" $@                                   
     eval $@                                         
}

workDir=$PWD  
                                                                       
logDir=${workDir}"/BATCHJOBS/v1.h2/"
command "mkdir -p $logDir"

castorDirIn="/castor/cern.ch/cms/store/caf/user/tdahms/HeavyIons/Onia/Data2010/v1/Skims/Prompt/150256-150619/"

castorDirOut="/castor/cern.ch/cms/store/user/tdahms/HeavyIons/Onia/Data2010/v1/Histos/Prompt/150256-150619/h2/"
command "rfmkdir -p  $castorDirOut"

inputfiles=""
j=0
jobNb=""

for files in `nsls $castorDirIn`
do
	inputfiles="inputFiles=rfio:"$castorDirIn$files;
	echo $inputfiles;
	jobNb=${j};
	let j=${j}+1;
	name="OniaSkim_allv2_${jobNb}"
	outfilename="Histos_${name}.root"
	secoutfilename="DataSet_${name}.root"

#Start to write the script
	cat > job_hionia_${jobNb}.sh << EOF
#!/bin/sh

function command(){                                  
     echo ">>>" \$@                                   
     eval \$@                                         
}

workDir=$PWD  
logDir="${logDir}"
castorDirOut="${castorDirOut}"

cd /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_5/src/
eval \`scramv1 runtime -sh\`
cd -
cp /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_5/src/HiAnalysis/HiOnia/test/hioniaanalyzer_cfg.py .
echo "before running cmsRun"
ls -l

command "cmsRun hioniaanalyzer_cfg.py ${inputfiles} outputFile=${outfilename} secondaryOutputFile=${secoutfilename} print 2>&1 | tee -a $logDir${name}.log"
echo "after running cmsRun"
ls -l
command "rfcp ${outfilename} $castorDirOut"
command "rfcp ${secoutfilename} $castorDirOut"
command "gzip -f $logDir${name}.log"
rm -f hioniaanalyzerpat_cfg.py

EOF

	chmod 755 job_hionia_${jobNb}.sh
	
	bsub -q 2nw -J $name /afs/cern.ch/user/t/tdahms/scratch0/HeavyIons2010/CMSSW_3_9_5/src/HiAnalysis/HiOnia/test/job_hionia_${jobNb}.sh


done




