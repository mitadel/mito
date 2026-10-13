import sys
import pandas as pd
import matplotlib.pyplot as plt

# the two formulations, with short aliases
CASES = {
    'continuous': 'continuous', 'cg': 'continuous',
    'discontinuous': 'discontinuous', 'dg': 'discontinuous',
}


def face_value(xs, ys, x_target):
    """The potential at a crack face: read off the mesh if it has a point there, as in the
    discontinuous case, and extrapolated from the two nearest points otherwise."""
    # the two points closest to the face, the nearest one last
    nearest = (xs - x_target).abs().sort_values().index[:2][::-1]
    x0, x1 = xs[nearest[0]], xs[nearest[1]]
    y0, y1 = ys[nearest[0]], ys[nearest[1]]
    if abs(x1 - x_target) < 1.0e-6 * abs(x_target):
        return float(y1)
    return float(y1 + (y1 - y0) / (x1 - x0) * (x_target - x1))


def plot_verification(case: str = 'discontinuous', ratio: float = 1000, h: float = 5e-5):
    ratio_tag = f'{ratio:g}'.replace('.', 'p')    # matches the C++ driver's ratio_tag

    csv_path = f'hybrid_dimensional_diffusion_{case}_ratio_{ratio_tag}.csv'
    df = pd.read_csv(csv_path).sort_values('physical_z').reset_index(drop=True)

    # the two half-domains; the crack occupies [-h, h] and is never meshed
    bottom = df[df['physical_z'] < 0]
    top = df[df['physical_z'] > 0]

    _, ax = plt.subplots(figsize=(8, 6))

    # the collapsed layer
    ax.axvspan(-h, h, color='lightgray', zorder=0)

    # each solution crosses the crack along a polyline through the two face values; in the
    # discontinuous case the bulk already reaches the faces and the connectors degenerate
    for column, color, style, width, label in (
            ('exact', '#c41e3a', 'solid', 1.5, 'Analytical solution'),
            ('numerical', '#0d47a1', (0, (6, 6)), 2, 'Numerical solution')):
        left = face_value(bottom['physical_z'], bottom[column], -h)
        right = face_value(top['physical_z'], top[column], h)
        ax.plot(
            bottom['physical_z'], bottom[column], color=color, linestyle=style, linewidth=width,
            label=label)
        ax.plot(top['physical_z'], top[column], color=color, linestyle=style, linewidth=width)
        ax.plot(
            [bottom['physical_z'].iloc[-1], -h, h, top['physical_z'].iloc[0]],
            [bottom[column].iloc[-1], left, right, top[column].iloc[0]],
            color=color, linestyle=style, linewidth=width)
        # markers only on the mesh points
        ax.plot(bottom['physical_z'], bottom[column], 'o', color=color, markersize=4)
        ax.plot(top['physical_z'], top[column], 'o', color=color, markersize=4)

    # boundaries between the solid electrolyte and the crack
    ax.axvline(-h, color='black', linestyle='--', linewidth=1.5)
    ax.axvline(h, color='black', linestyle='--', linewidth=1.5, label='boundary SE and crack')

    ax.set_xlabel(r'$y\;[\mathrm{m}]$')
    ax.set_ylabel(r'$\phi\;[\mathrm{V}]$')
    ax.set_title(
        rf'Conductivity $\kappa_m/\kappa_{{SE}} = {ratio}$'
        r' and crack width $w = 1\cdot e^{-4}$ m')
    ax.set_xlim(-1.5e-4, 1.5e-4)
    ax.set_ylim(-5.5, 1.5)
    ax.legend(loc='upper left')
    ax.ticklabel_format(style='sci', axis='x', scilimits=(0, 0))

    plt.tight_layout()
    plt.savefig(f'hybrid_dimensional_diffusion_{case}_ratio_{ratio_tag}.png', format='png')
    plt.show()


if __name__ == "__main__":
    case = CASES[sys.argv[1].lower()] if len(sys.argv) > 1 else 'discontinuous'
    ratio = float(sys.argv[2]) if len(sys.argv) > 2 else 1000
    plot_verification(case=case, ratio=ratio, h=5e-5)
