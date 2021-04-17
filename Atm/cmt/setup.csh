# echo "setup Atm v0 in /afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtAtmtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtAtmtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  -no_cleanup $* >${cmtAtmtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  -no_cleanup $* >${cmtAtmtempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtAtmtempfile}
  unset cmtAtmtempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtAtmtempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtAtmtempfile}
unset cmtAtmtempfile
exit $cmtsetupstatus

