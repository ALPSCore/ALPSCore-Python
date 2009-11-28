#
# libalps
#

set(ALPS_SOURCES "")
set(ALPS_SHARED_CPPFLAGS ALPS_EXPORTS=1)
set(ALPS_STATIC_CPPFLAGS "")

# alps
set(ALPS_SOURCES
    # alps 
    copyright.C
    # alea
    alea/nan.C alea/observable.C alea/observableset.C
    # expression
    expression/evaluator.C
    # lattice
    lattice/disorder.C lattice/latticedescriptor.C lattice/latticegraphdescriptor.C
    lattice/latticelibrary.C lattice/unitcell.C
    # model
    model/bondterm.C model/globaloperator.C model/modellibrary.C model/siteterm.C
    # osiris
    osiris/buffer.C osiris/dump.C osiris/dumparchive.C osiris/os.C osiris/process.C osiris/comm.C
    osiris/mpdump.C osiris/xdrdump.C osiris/xdrcore.C
    # parameter
    parameter/parameter.C parameter/parameterlist.C parameter/parameters.C
    # parapack
    parapack/clone.C parapack/clone_info.C parapack/filelock.C parapack/logger.C
    parapack/job.C parapack/mc_worker.C parapack/measurement.C parapack/option.C parapack/rng_helper.C
    parapack/queue.C parapack/scheduler_comm.C parapack/scheduler_sgl.C
    parapack/types.C parapack/util.C parapack/worker_factory.C
    # parser
    parser/parser.C parser/xmlattributes.C parser/xmlhandler.C parser/xmlparser.C
    parser/xmlstream.C parser/xslt_path.C
    # random
    random/rngfactory.C
    # scheduler
    scheduler/abstract_task.C scheduler/factory.C scheduler/info.C
    scheduler/master_scheduler.C scheduler/measurement_operators.C
    scheduler/montecarlo.C scheduler/mpp_scheduler.C scheduler/options.C
    scheduler/remote_task.C scheduler/remote_worker.C scheduler/scheduler.C
    scheduler/serial_scheduler.C scheduler/signal.C scheduler/single_scheduler.C
    scheduler/slave_task.C scheduler/task.C scheduler/worker.C scheduler/workertask.C
)

add_library(alps ${ALPS_SOURCES})
target_link_libraries(alps boost)
if(BUILD_SHARED_LIBS)
  set_target_properties(alps PROPERTIES COMPILE_DEFINITIONS "${ALPS_SHARED_CPPFLAGS}")
else(BUILD_SHARED_LIBS)
  set_target_properties(alps PROPERTIES COMPILE_DEFINITIONS "${ALPS_STATIC_CPPFLAGS}")
endif(BUILD_SHARED_LIBS)

#######################################################################
# install
#######################################################################
install(TARGETS alps
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)