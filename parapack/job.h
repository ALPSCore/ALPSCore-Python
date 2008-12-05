/*****************************************************************************
*
* ALPS Project: Algorithms and Libraries for Physics Simulations
*
* ALPS Libraries
*
* Copyright (C) 1997-2008 by Synge Todo <wistaria@comp-phys.org>
*
* This software is part of the ALPS libraries, published under the ALPS
* Library License; you can use, redistribute it and/or modify it under
* the terms of the license, either version 1 or (at your option) any later
* version.
* 
* You should have received a copy of the ALPS Library License along with
* the ALPS Libraries; see the file LICENSE.txt. If not, the license is also
* available from http://alps.comp-phys.org/.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

#ifndef PARAPACK_JOB_H
#define PARAPACK_JOB_H

#include "clone_info.h"
#include "integer_range.h"
#include "types.h"
#include <alps/parser/xmlstream.h>
#include <alps/scheduler.h>
#include <deque>
#include <set>
#include <vector>

namespace alps {

class job_task_xml_handler;
class job_xml_handler;

class task {
public:
  typedef integer_range<uint32_t> range_type;

  task();
  task(boost::filesystem::path const& file);

  task_status_t status() const { return status_; }
  tid_t task_id() const { return task_id_; }
  bool on_memory() const;

  range_type const& num_clones() const;
  uint32_t num_running() const;
  uint32_t num_suspended() const;
  uint32_t num_finished() const;
  uint32_t num_started() const;
  double progress() const { return progress_; }
  double weight() const { return weight_; }

  void load();
  void save() const;
  void save_observable() const;
  void halt();

  void check_parameter();

  bool can_dispatch() const;
  template<typename PROXY>
  std::pair<cid_t, bool> dispatch_clone(PROXY& proxy,
    process_group const& procs = process_group()) {
    if (!on_memory()) load();
    if (!can_dispatch())
      boost::throw_exception(std::logic_error("no more clones can be dispatched"));
    bool is_new;
    cid_t cid;
    Process master = (procs.process_list.size() ? procs.process_list[0] : Process());
    if (num_suspended()) {
      is_new = false;
      cid = *suspended_.begin();
      suspended_.erase(suspended_.begin());
      clone_status_[cid] = clone_status::Running;
      clone_master_[cid] = master;
    } else {
      is_new = true;
      cid = clone_status_.size();
      clone_status_.push_back(clone_status::Running);
      clone_master_.push_back(master);
      clone_info_.push_back(clone_info());
    }
    running_.insert(cid);
    proxy.start(task_id_, cid, procs, params_, basedir_, base_, is_new);
    boost::tie(weight_, dump_weight_) = calc_weight();
    status_ = calc_status();
    return std::make_pair(cid, is_new);
  }

  bool is_running(cid_t cid) const {
    return clone_status_[cid] == clone_status::Running;
  }

  template<typename PROXY>
  void checkpoint(PROXY& proxy, cid_t cid) const {
    if (clone_status_[cid] == clone_status::Running)
      proxy.checkpoint(clone_master_[cid]);
  }

  template<typename PROXY>
  void update_info(PROXY& proxy, cid_t cid) const {
    if (clone_status_[cid] == clone_status::Running)
      proxy.update_info(clone_master_[cid]);
  }

  template<typename PROXY>
  void suspend_clones(PROXY& proxy) {
    if (on_memory()) {
      BOOST_FOREACH(cid_t cid, running_) {
        if (clone_status_[cid] == clone_status::Running) {
          clone_status_[cid] = clone_status::Stopping;
          proxy.suspend(clone_master_[cid]);
        }
      }
    }
  }

  template<typename PROXY>
  void halt_clone(PROXY& proxy, cid_t cid) {
    if (clone_status_[cid] != clone_status::Idling)
      boost::throw_exception(std::logic_error("clone is not idling"));
    clone_status_[cid] = clone_status::Stopping;
    proxy.halt(clone_master_[cid]);
  }

  void info_updated(cid_t cid, clone_info const& info);
  void clone_suspended(cid_t cid, clone_info const& info);
  void clone_halted(cid_t cid);

  void evaluate();

  void write_xml_summary(oxstream& os) const;
  void write_xml_archive(oxstream& os) const;

  std::string const& file_in_str() const { return file_in_str_; }
  std::string const& file_out_str() const { return file_out_str_; }

protected:
  double calc_progress() const;
  std::pair<double, double> calc_weight() const;
  task_status_t calc_status() const;

private:
  friend class job_task_xml_handler;
  friend class job_tasks_xml_handler;

  // these parameters will be read from job xml file
  task_status_t status_;
  tid_t task_id_;
  double progress_;
  double weight_;
  double dump_weight_;
  boost::filesystem::path basedir_;
  std::string file_in_str_;
  std::string file_out_str_;
  std::string base_;

  // these parameters will be read from simulation xml file
  Parameters params_;
  std::vector<ObservableSet> obs_;

  range_type num_clones_;
  std::deque<clone_info> clone_info_;
  std::deque<clone_status_t> clone_status_;
  std::deque<Process> clone_master_;
  std::set<cid_t> running_;   // id of running clones
  std::set<cid_t> suspended_; // id of suspended clones
  std::set<cid_t> finished_;  // id of finished clones
};


} // end namespace alps

#endif // PARAPACK_JOB_H