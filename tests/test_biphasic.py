import numpy as np
import tumourmodel as tm


def _simulate_bi(theta1, theta2, *, tau_days, T, seed, start_volume, page=5):
    """Call simulate_biphasic handling either signature (with/without `page`)."""
    try:
        return tm.simulate_biphasic(
            theta1,
            theta2,
            tau_days=tau_days,
            T=T,
            seed=seed,
            start_volume=start_volume,
            page=page,
        )
    except TypeError:
        return tm.simulate_biphasic(
            theta1, theta2, tau_days=tau_days, T=T, seed=seed, start_volume=start_volume
        )


def test_biphasic_available():
    assert hasattr(
        tm, "simulate_biphasic"
    ), "simulate_biphasic not exposed in Python API"


def test_biphasic_shape_and_determinism():
    T, tau, seed, start = 10, 5, 123, 50.0
    theta1 = [0.05, 1e-2, 30, 48]  # p0, ppsc, dmax, gage
    theta2 = [0.10, 5e-3, 25, 36]
    y1 = _simulate_bi(theta1, theta2, tau_days=tau, T=T, seed=seed, start_volume=start)
    y2 = _simulate_bi(theta1, theta2, tau_days=tau, T=T, seed=seed, start_volume=start)
    assert isinstance(y1, np.ndarray) and y1.shape == (T,)
    assert np.allclose(y1, y2)


def test_biphasic_reduces_to_monophasic_when_tau_ge_T():
    T, tau, seed, start, page = 8, 8, 777, 50.0, 5
    theta1 = [0.03, 1e-2, 28, 40]
    y_bi = _simulate_bi(
        theta1, theta1, tau_days=tau, T=T, seed=seed, start_volume=start, page=page
    )
    y_mono = tm.simulate(theta1 + [page], T=T, seed=seed, start_volume=start)
    assert np.allclose(y_bi, y_mono)


def test_switch_changes_trajectory():
    T, seed, start = 12, 999, 50.0
    theta1 = [0.05, 1e-2, 30, 48]
    theta2 = [0.20, 1e-3, 20, 24]
    y_no_switch = _simulate_bi(
        theta1, theta2, tau_days=T, T=T, seed=seed, start_volume=start
    )
    y_switch = _simulate_bi(
        theta1, theta2, tau_days=3, T=T, seed=seed, start_volume=start
    )
    assert not np.allclose(y_no_switch, y_switch)
