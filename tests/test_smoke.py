import numpy as np
import tumourmodel as tm


def test_simulate():
    theta = [0.05, 0.01, 30, 48, 5]  # p0, psc, dmax, gage, page
    y = tm.simulate(theta, T=5, seed=123, start_volume=50.0)
    print("Simulation output:", y)
    assert isinstance(y, np.ndarray)
    assert y.shape == (5,)


if __name__ == "__main__":
    test_simulate()
    print("All tests passed.")
