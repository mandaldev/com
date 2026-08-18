// com - bunch of C++ common utils based on stdc++ and enchance stdc++
// Copyright (C) 2026  Mandal Dev
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef COM_SRC_PROC_PROCESS_HPP
#define COM_SRC_PROC_PROCESS_HPP
#include <string>

namespace Com::Proc::Process {

struct Stat {
	/// The process ID.
	int pid = 0;
	/// The filename of the executable, in parentheses.
	/// Strings longer than TASK_COMM_LEN (16) characters
	/// (including the terminating null byte) are silently
	/// truncated.  This is visible whether or not the
	/// executable is swapped out.
	std::string comm;
	/// - R: Running
	/// - S: Sleeping in an interruptible wait
	/// - D: Waiting in uninterruptible disk sleep
	/// - Z: Zombie
	/// - T: Stopped (on a signal) or (before Linux 2.6.33) trace stopped
	/// - t: Tracing stop (Linux 2.6.33 onward)
	/// - W: Paging (only before Linux 2.6.0)
	/// - X: Dead (from Linux 2.6.0 onward)
	/// - x: Dead (Linux 2.6.33 to 3.13 only)
	/// - K: Wakekill (Linux 2.6.33 to 3.13 only)
	/// - W: Waking (Linux 2.6.33 to 3.13 only)
	/// - P: Parked (Linux 3.9 to 3.13 only)
	/// - I: Idle (Linux 4.14 onward)
	char state = 0;
	/// The PID of the parent of this process.
	int ppid = 0;
	/// The process group ID of the process.
	int pgrp = 0;
	/// The session ID of the process.
	int session = 0;
	/// The controlling terminal of the process.  (The minor
	/// device number is contained in the combination of
	/// bits 31 to 20 and 7 to 0; the major device number is
	/// in bits 15 to 8.)
	int tty_nr = 0;
	/// The ID of the foreground process group of the controlling terminal of the process.
	int tpgid = 0;
	/// The kernel flags word of the process.  For bit
	/// meanings, see the PF_* defines in the Linux kernel
	/// source file include/linux/sched.h.  Details depend
	/// on the kernel version.
	/// The format for this field was %lu before Linux 2.6.
	unsigned flags = 0;
	/// The number of minor faults the process has made
	/// which have not required loading a memory page from disk.
	unsigned long minflt = 0;
	/// The number of minor faults that the process's waited-for children have made.
	unsigned long cminflt = 0;
	/// The number of major faults the process has made which have required loading a memory page from disk.
	unsigned long majflt = 0;
	/// The number of major faults that the process's waited-for children have made.
	unsigned long cmajflt = 0;
	/// Amount of time that this process has been scheduled
	/// in user mode, measured in clock ticks (divide by
	/// sysconf(_SC_CLK_TCK)).  This includes guest time,
	/// guest_time (time spent running a virtual CPU, see
	/// below), so that applications that are not aware of
	/// the guest time field do not lose that time from
	/// their calculations.
	unsigned long utime = 0;
	/// Amount of time that this process has been scheduled
	/// in kernel mode, measured in clock ticks (divide by
	/// sysconf(_SC_CLK_TCK)).
	unsigned long stime = 0;
	/// Amount of time that this process's waited-for
	/// children have been scheduled in user mode, measured
	/// in clock ticks (divide by sysconf(_SC_CLK_TCK)).
	/// (See also times(2).)  This includes guest time,
	/// cguest_time (time spent running a virtual CPU, see
	/// below).
	unsigned long cutime = 0;
	/// Amount of time that this process's waited-for
	/// children have been scheduled in kernel mode,
	/// measured in clock ticks (divide by sysconf(_SC_CLK_TCK)).
	long cstime = 0;
	/// (Explanation for Linux 2.6) For processes running a
	/// real-time scheduling policy (policy below; see
	/// sched_setscheduler(2)), this is the negated
	/// scheduling priority, minus one; that is, a number in
	/// the range -2 to -100, corresponding to real-time
	/// priorities 1 to 99.  For processes running under a
	/// non-real-time scheduling policy, this is the raw
	/// nice value (setpriority(2)) as represented in the
	/// kernel.  The kernel stores nice values as numbers in
	/// the range 0 (high) to 39 (low), corresponding to the
	/// user-visible nice range of -20 to 19.
	/// Before Linux 2.6, this was a scaled value based on
	/// the scheduler weighting given to this process.
	long priority = 0;
	/// The nice value (see setpriority(2)), a value in the
	/// range 19 (low priority) to -20 (high priority).
	long nice = 0;
	/// Number of threads in this process (since Linux 2.6).
	/// Before Linux 2.6, this field was hard coded to 0 as
	/// a placeholder for an earlier removed field.
	long num_threads = 0;
	/// The time in jiffies before the next SIGALRM is sent
	/// to the process due to an interval timer.  Since
	/// Linux 2.6.17, this field is no longer maintained,
	/// and is hard coded as 0.
	long itrealvalue = 0;
	/// The time the process started after system boot.
	/// Before Linux 2.6, this value was expressed in
	/// jiffies.  Since Linux 2.6, the value is expressed in
	/// clock ticks (divide by sysconf(_SC_CLK_TCK)).
	/// The format for this field was %lu before Linux 2.6.
	unsigned long long starttime = 0;
	/// Virtual memory size in bytes.
	unsigned long vsize = 0;
	/// Resident Set Size: number of pages the process has
	/// in real memory.  This is just the pages which count
	/// toward text, data, or stack space.  This does not
	/// include pages which have not been demand-loaded in,
	/// or which are swapped out.  This value is inaccurate;
	/// see /proc/pid/statm below.
	long rss = 0;
	/// Current soft limit in bytes on the rss of the
	/// process; see the description of RLIMIT_RSS in
	/// getrlimit(2).
	unsigned long rsslim = 0;
	/// The address above which program text can run.
	unsigned long startcode = 0;
	/// The address below which program text can run.
	unsigned long endcode = 0;
	/// The address of the start (i.e., bottom) of the stack.
	unsigned long startstack = 0;
	/// The current value of ESP (stack pointer), as found in the kernel stack page for the process.
	unsigned long kstkesp = 0;
	/// The current EIP (instruction pointer).
	unsigned long kstkeip = 0;
	/// The bitmap of pending signals, displayed as a
	/// decimal number.  Obsolete, because it does not
	/// provide information on real-time signals; use
	/// /proc/pid/status instead.
	unsigned long signal = 0;
	/// The bitmap of blocked signals, displayed as a
	/// decimal number.  Obsolete, because it does not
	/// provide information on real-time signals; use
	/// /proc/pid/status instead.
	unsigned long blocked = 0;
	/// The bitmap of ignored signals, displayed as a
	/// decimal number.  Obsolete, because it does not
	/// provide information on real-time signals; use
	/// /proc/pid/status instead.
	unsigned long sigignore = 0;
	/// The bitmap of caught signals, displayed as a decimal
	/// number.  Obsolete, because it does not provide
	/// information on real-time signals; use
	/// /proc/pid/status instead.
	unsigned long sigcatch = 0;
	/// This is the "channel" in which the process is
	/// waiting.  It is the address of a location in the
	/// kernel where the process is sleeping.  The
	/// corresponding symbolic name can be found in
	/// /proc/pid/wchan.
	unsigned long wchan = 0;
	/// Number of pages swapped (not maintained).
	unsigned long nswap = 0;
	/// Cumulative nswap for child processes (not maintained).
	unsigned long cnswap = 0;
	/// (since Linux 2.1.22) Signal to be sent to parent when we die.
	int exit_signal = 0;
	/// (since Linux 2.2.8) CPU number last executed on.
	int processor = 0;
	/// (since Linux 2.5.19) Real-time scheduling priority, a number in the range
	/// 1 to 99 for processes scheduled under a real-time
	/// policy, or 0, for non-real-time processes (see sched_setscheduler(2)).
	unsigned rt_priority = 0;
	/// (since Linux 2.5.19) Scheduling policy (see sched_setscheduler(2)).
	/// Decode using the SCHED_* constants in linux/sched.h.
	/// The format for this field was %lu before Linux 2.6.22.
	unsigned policy = 0;
	/// (since Linux 2.6.18) Aggregated block I/O delays, measured in clock ticks (centiseconds).
	unsigned long long delayacct_blkio_ticks = 0;
	/// (since Linux 2.6.24) Guest time of the process (time spent running a
	/// virtual CPU for a guest operating system), measured
	/// in clock ticks (divide by sysconf(_SC_CLK_TCK)).
	unsigned long guest_time = 0;
	/// (since Linux 2.6.24)
	/// Guest time of the process's children, measured in
	/// clock ticks (divide by sysconf(_SC_CLK_TCK)).
	long cguest_time = 0;
	/// (since Linux 3.3) [PT]
	/// Address above which program initialized and
	/// uninitialized (BSS) data are placed.
	unsigned long start_data = 0;
	/// (since Linux 3.3) [PT]
	/// Address below which program initialized and
	/// uninitialized (BSS) data are placed.
	unsigned long end_data = 0;
	/// (since Linux 3.3) [PT]
	/// Address above which program heap can be expanded
	/// with brk(2).
	unsigned long start_brk = 0;
	/// (since Linux 3.5) [PT]
	/// Address above which program command-line arguments
	/// (argv) are placed.
	unsigned long arg_start = 0;
	/// (since Linux 3.5) [PT] Address below program command-line arguments (argv) are placed.
	unsigned long arg_end = 0;
	/// (since Linux 3.5) [PT] Address above which program environment is placed.
	unsigned long env_start = 0;
	/// (since Linux 3.5) [PT] Address below which program environment is placed.
	unsigned long env_end = 0;
	/// (since Linux 3.5) [PT] The thread's exit status in the form reported by waitpid(2).
	int exit_code = 0;
};

struct Statm {
	/// total program size (same as VmSize in /proc/pid/status)
	unsigned long size = 0;
	/// resident set size (inaccurate; same as VmRSS in /proc/pid/status)
	unsigned long resident = 0;
	/// number of resident shared pages (i.e., backed by a file)
	/// (inaccurate; same as RssFile+RssShmem in /proc/pid/status)
	unsigned long shared = 0;
	/// text (code)
	unsigned long text = 0;
	/// library (unused since Linux 2.6; always 0)
	unsigned long lib = 0;
	/// data + stack
	unsigned long data = 0;
	/// dirty pages (unused since Linux 2.6; always 0)
	unsigned long dt = 0;
};

Stat stat(std::string const &pid);

Stat stat(int pid);

Statm statm(std::string const &pid);

Statm statm(int pid);

unsigned long page_size();

}
#endif
