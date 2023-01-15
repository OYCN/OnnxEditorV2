/**
 * @file main.cpp
 * @brief
 * @author opluss (opluss@qq.com)
 *
 * @copyright Copyright (c) 2022  opluss
 *
 * @par Modify log:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-08-13 <td>1.0     <td>opluss     <td>Initial
 * </table>
 */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "gui/gui.h"

const char* usage = R"(
  Onnx edit utils
)";

DEFINE_bool(nogui, false, "disable gui");

void initializer_args(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = 1;
  FLAGS_colorlogtostderr = true;
  google::InstallFailureSignalHandler();
  gflags::SetUsageMessage(usage);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

int main(int argc, char* argv[]) {
  initializer_args(argc, argv);

  bool ret = true;
  if (!FLAGS_nogui) {
    ret = gui::gui_on(argc, argv);
  }

  return ret;
}
