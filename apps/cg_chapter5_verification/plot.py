import sys
import pandas as pd
import matplotlib.pyplot as plt

def plot_verification(ratio: float = 1000, h: float = 5e-5):
    if ratio < 1:
        ratio_str = str(ratio).replace('.', 'p')  # so e.g. 0.01 becomes '0p01' for the filename
    else:
        ratio_str = str(int(ratio))

    csv_path = f'dg_chapter5_verification_ratio_{ratio_str}.csv'
    df = pd.read_csv(csv_path).sort_values('physical_z').reset_index(drop=True)

    bottom = df[df['physical_z'] < -h]
    top    = df[df['physical_z'] >  h]

    def extrapolate(xs, ys, x_target):
        """Linear extrapolation using the two points closest to x_target."""
        slope = (ys.iloc[-1] - ys.iloc[-2]) / (xs.iloc[-1] - xs.iloc[-2])
        return float(ys.iloc[-1] + slope * (x_target - xs.iloc[-1]))

    # extrapolated values at the crack faces
    ana_left  = extrapolate(bottom['physical_z'], bottom['exact'],     -h)
    ana_right = extrapolate(top   ['physical_z'], top   ['exact'],      h)
    num_left  = extrapolate(bottom['physical_z'], bottom['numerical'], -h)
    num_right = extrapolate(top   ['physical_z'], top   ['numerical'],  h)

    _, ax = plt.subplots(figsize=(8, 6))

    # grey zone
    ax.axvspan(-h, h, color='lightgray', zorder=0)

    # analytical (red, solid)
    # bulk segments + extrapolated endpoint, then interface segment (no markers)
    ax.plot(
        list(bottom['physical_z']) + [-h],
        list(bottom['exact'])      + [ana_left],
        color='#c41e3a', linewidth=1.5, label='Analytical solution',
    )
    ax.plot(
        [h] + list(top['physical_z']),
        [ana_right] + list(top['exact']),
        color='#c41e3a', linewidth=1.5,
    )
    ax.plot([-h, h], [ana_left, ana_right], color='#c41e3a', linewidth=1.5)
    # markers only on real data points
    ax.plot(bottom['physical_z'], bottom['exact'], 'o', color='#c41e3a',  markersize=4)
    ax.plot(top   ['physical_z'], top   ['exact'], 'o', color='#c41e3a',  markersize=4)

    # numerical (blue, dashed)
    ax.plot(
        list(bottom['physical_z']) + [-h],
        list(bottom['numerical'])  + [num_left],
        color='#0d47a1', linestyle=(0, (6, 6)), linewidth=2, label='Numerical solution',
    )
    ax.plot(
        [h] + list(top['physical_z']),
        [num_right] + list(top['numerical']),
        color='#0d47a1', linestyle=(0, (6, 6)), linewidth=2,
    )
    ax.plot([-h, h], [num_left, num_right], color='#0d47a1', linestyle=(0, (6, 6)), linewidth=2)
    ax.plot(bottom['physical_z'], bottom['numerical'], 'o', color='#0d47a1', markersize=4)
    ax.plot(top   ['physical_z'], top   ['numerical'], 'o', color='#0d47a1', markersize=4)

    # boundary lines
    ax.axvline(-h, color='black', linestyle='--', linewidth=1.5)
    ax.axvline( h, color='black', linestyle='--', linewidth=1.5, label='boundary SE and crack')

    ax.set_xlabel(r'$y\;[\mathrm{m}]$')
    ax.set_ylabel(r'$\phi\;[\mathrm{V}]$')
    ax.set_title(
        rf'Conductivity $\kappa_m/\kappa_{{SE}} = {ratio}$'
        r' and crack width $w = 1\cdot e^{-4}$ m'
    )
    ax.set_xlim(-1.5e-4, 1.5e-4)
    ax.set_ylim(-5.5, 1.5)
    ax.legend(loc='upper left')
    ax.ticklabel_format(style='sci', axis='x', scilimits=(0, 0))

    plt.tight_layout()
    plt.savefig(f'verification_ratio_{ratio_str}.png', format='png')
    plt.show()


if __name__ == "__main__":
    ratio = float(sys.argv[1]) if len(sys.argv) > 1 else 1000
    plot_verification(ratio=ratio, h=5e-5)