# echo "cleanup Atm v0 in /afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtAtmtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtAtmtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  $* >${cmtAtmtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  $* >${cmtAtmtempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtAtmtempfile}
  unset cmtAtmtempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtAtmtempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtAtmtempfile}
unset cmtAtmtempfile
return $cmtcleanupstatus

