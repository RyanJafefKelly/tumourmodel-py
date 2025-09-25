#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <cstring>

#include "cpp/Model.h"

namespace py = pybind11;

static int parse_page_or_default(const py::sequence& theta, int default_page=5) {
    if (py::len(theta) == 5) return int(py::int_(theta[4]));
    if (py::len(theta) == 4) return default_page;
    throw std::runtime_error("theta must have 4 or 5 entries: [p0, psc, dmax, gage(, page)]");
}

static void parse_theta(const py::sequence& theta, double& p0, double& psc, int& dmax, int& gage, int& page, int default_page=5) {
    if (py::len(theta) != 4 && py::len(theta) != 5)
        throw std::runtime_error("theta must have 4 or 5 entries: [p0, psc, dmax, gage(, page)]");
    p0   = py::float_(theta[0]);
    psc  = py::float_(theta[1]);
    dmax = py::int_(theta[2]);
    gage = py::int_(theta[3]);
    page = parse_page_or_default(theta, default_page);
}

static py::array_t<double>
simulate_py(const py::sequence& theta, int T, std::uint64_t seed, double start_volume)
{
    double p0, psc; int dmax, gage, page;
    parse_theta(theta, p0, psc, dmax, gage, page);

    std::vector<double> vols =
        FullSimulation_seeded(p0, psc, dmax, gage, page, start_volume, T, seed);

    auto out = py::array_t<double>(vols.size());
    std::memcpy(out.mutable_data(), vols.data(), vols.size() * sizeof(double));
    return out;
}

static py::array_t<double>
simulate_biphasic_py(const py::sequence& theta1, const py::sequence& theta2,
                     int tau_days, int T, std::uint64_t seed, double start_volume)
{
    double p0_1, psc_1; int dmax_1, gage_1, page1;
    double p0_2, psc_2; int dmax_2, gage_2, page2;

    parse_theta(theta1, p0_1, psc_1, dmax_1, gage_1, page1);
    parse_theta(theta2, p0_2, psc_2, dmax_2, gage_2, page2);

    // Use a single page for both phases (paper’s BVCBM doesn’t calibrate page)
    if (page1 != page2) {
        throw std::runtime_error("page differs across phases; pass the same page or omit it to use the default.");
    }
    int page = page1;

    std::vector<double> vols =
        FullSimulation_biphasic_seeded(p0_1, psc_1, dmax_1, gage_1, page,
                                       p0_2, psc_2, dmax_2, gage_2, tau_days,
                                       start_volume, T, seed);

    auto out = py::array_t<double>(vols.size());
    std::memcpy(out.mutable_data(), vols.data(), vols.size() * sizeof(double));
    return out;
}

PYBIND11_MODULE(_core, m) {
    m.doc() = "Tumour model bindings";
    m.def("simulate", &simulate_py,
          py::arg("theta"), py::arg("T"), py::arg("seed"),
          py::arg("start_volume") = 50.0,
          "Monophasic: returns tumour volumes for T days.");
    m.def("simulate_biphasic", &simulate_biphasic_py,
          py::arg("theta1"), py::arg("theta2"), py::arg("tau_days"), py::arg("T"), py::arg("seed"),
          py::arg("start_volume") = 100.0,
          "Biphasic: theta1/theta2 are [p0, psc, dmax, gage(, page)], switch at tau_days.");
}
