#-- start of make_header -----------------

#====================================
#  Application Atm
#
#   Generated Fri Apr 16 10:03:19 2021  by huyuxiang
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_Atm_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Atm_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Atm

Atm_tag = $(tag)

#cmt_local_tagfile_Atm = $(Atm_tag)_Atm.make
cmt_local_tagfile_Atm = $(bin)$(Atm_tag)_Atm.make

else

tags      = $(tag),$(CMTEXTRATAGS)

Atm_tag = $(tag)

#cmt_local_tagfile_Atm = $(Atm_tag).make
cmt_local_tagfile_Atm = $(bin)$(Atm_tag).make

endif

include $(cmt_local_tagfile_Atm)
#-include $(cmt_local_tagfile_Atm)

ifdef cmt_Atm_has_target_tag

cmt_final_setup_Atm = $(bin)setup_Atm.make
cmt_dependencies_in_Atm = $(bin)dependencies_Atm.in
#cmt_final_setup_Atm = $(bin)Atm_Atmsetup.make
cmt_local_Atm_makefile = $(bin)Atm.make

else

cmt_final_setup_Atm = $(bin)setup.make
cmt_dependencies_in_Atm = $(bin)dependencies.in
#cmt_final_setup_Atm = $(bin)Atmsetup.make
cmt_local_Atm_makefile = $(bin)Atm.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)Atmsetup.make

#Atm :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'Atm'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Atm/
#Atm::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of application_header

Atm :: dirs  $(bin)Atm${application_suffix}
	$(echo) "Atm ok"

cmt_Atm_has_prototypes = 1

#--------------------------------------

ifdef cmt_Atm_has_prototypes

Atmprototype :  ;

endif

Atmcompile : $(bin)Atm.o ;

#-- end of application_header
#-- start of application

$(bin)Atm${application_suffix} :: $(bin)Atm.o $(use_stamps) $(Atm_stamps) $(Atmstamps) $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)Atm.o $(cmt_installarea_linkopts) $(Atm_use_linkopts) $(Atmlinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
Atminstallname = Atm${application_suffix}

Atm :: Atminstall ;

install :: Atminstall ;

Atminstall :: $(install_dir)/$(Atminstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(Atminstallname) :: $(bin)$(Atminstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Atminstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##Atmclean :: Atmuninstall

uninstall :: Atmuninstall ;

Atmuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Atminstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (Atm.make) Removing installed files"
#-- end of application
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),Atmclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),Atmprototype)

$(bin)Atm_dependencies.make : $(use_requirements) $(cmt_final_setup_Atm)
	$(echo) "(Atm.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)Atm.cc -end_all $(includes) $(app_Atm_cppflags) $(lib_Atm_cppflags) -name=Atm $? -f=$(cmt_dependencies_in_Atm) -without_cmt

-include $(bin)Atm_dependencies.make

endif
endif
endif

Atmclean ::
	$(cleanup_silent) \rm -rf $(bin)Atm_deps $(bin)Atm_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp ------

ifneq (,)

ifneq ($(MAKECMDGOALS),Atmclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Atm.d

$(bin)$(binobj)Atm.d :

$(bin)$(binobj)Atm.o : $(cmt_final_setup_Atm)

$(bin)$(binobj)Atm.o : $(src)Atm.cc
	$(cpp_echo) $(src)Atm.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(Atm_pp_cppflags) $(app_Atm_pp_cppflags) $(Atm_pp_cppflags) $(use_cppflags) $(Atm_cppflags) $(app_Atm_cppflags) $(Atm_cppflags) $(Atm_cc_cppflags)  $(src)Atm.cc
endif
endif

else
$(bin)Atm_dependencies.make : $(Atm_cc_dependencies)

$(bin)Atm_dependencies.make : $(src)Atm.cc

$(bin)$(binobj)Atm.o : $(Atm_cc_dependencies)
	$(cpp_echo) $(src)Atm.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(Atm_pp_cppflags) $(app_Atm_pp_cppflags) $(Atm_pp_cppflags) $(use_cppflags) $(Atm_cppflags) $(app_Atm_cppflags) $(Atm_cppflags) $(Atm_cc_cppflags)  $(src)Atm.cc

endif

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: Atmclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(Atm.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

Atmclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) application Atm
	-$(cleanup_silent) cd $(bin); /bin/rm -f Atm${application_suffix}
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects Atm
	-$(cleanup_silent) /bin/rm -f $(bin)Atm.o
	-$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)Atm.o) $(patsubst %.o,%.dep,$(bin)Atm.o) $(patsubst %.o,%.d.stamp,$(bin)Atm.o)
	-$(cleanup_silent) cd $(bin); /bin/rm -rf Atm_deps Atm_dependencies.make
#-- end of cleanup_objects ------
