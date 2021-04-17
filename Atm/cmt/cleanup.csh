# echo "cleanup Atm v0 in /afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtAtmtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtAtmtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  $* >${cmtAtmtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  $* >${cmtAtmtempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtAtmtempfile}
  unset cmtAtmtempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtAtmtempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtAtmtempfile}
unset cmtAtmtempfile
exit $cmtcleanupstatus

