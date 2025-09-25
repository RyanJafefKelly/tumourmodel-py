#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "cpp/Model.h"

namespace py = pybind11;

static py::array_t<double>
simulate_py(const py::sequence& theta, int T, uint64_t seed, double start_volume)
{
    if (py::len(theta) != 5)
        throw std::runtime_error("theta must have 5 entries: [p0, psc, dmax, gage, page]");

    double p0   = py::float_(theta[0]);
    double psc  = py::float_(theta[1]);
    int    dmax = py::int_(theta[2]);
    int    gage = py::int_(theta[3]);
    int    page = py::int_(theta[4]);

    std::vector<double> vols =
        FullSimulation_seeded(p0, psc, dmax, gage, page, start_volume, T, seed);

    auto out = py::array_t<double>(vols.size());
    std::memcpy(out.mutable_data(), vols.data(), vols.size() * sizeof(double));
    return out;
}

PYBIND11_MODULE(_core, m) {
    m.doc() = "Tumour model bindings";
    m.def("simulate", &simulate_py,
          py::arg("theta"), py::arg("T"), py::arg("seed"), py::arg("start_volume") = 50.0,
          "Run the tumour model and return tumour volumes (length T).");
}
