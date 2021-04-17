# echo "setup Atm v0 in /afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtAtmtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtAtmtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  -no_cleanup $* >${cmtAtmtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=Atm -version=v0 -path=/afs/ihep.ac.cn/users/h/huyuxiang/junofs/juno_centos7/offline/Generator  -no_cleanup $* >${cmtAtmtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtAtmtempfile}
  unset cmtAtmtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtAtmtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtAtmtempfile}
unset cmtAtmtempfile
return $cmtsetupstatus

