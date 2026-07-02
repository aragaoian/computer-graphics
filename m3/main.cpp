#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

Vec3 operator+(const Vec3 &a, const Vec3 &b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 operator-(const Vec3 &a, const Vec3 &b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
Vec3 operator*(const Vec3 &v, double s) { return {v.x * s, v.y * s, v.z * s}; }
Vec3 operator/(const Vec3 &v, double s) { return {v.x / s, v.y / s, v.z / s}; }

double dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 cross(const Vec3 &a, const Vec3 &b)
{
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

double length(const Vec3 &v)
{
    return std::sqrt(dot(v, v));
}

Vec3 normalize(const Vec3 &v)
{
    const double len = length(v);
    if (len < 1e-12) {
        return {0.0, 1.0, 0.0};
    }
    return v / len;
}

struct Mesh {
    std::vector<Vec3> vertices;
    std::vector<std::array<int, 3>> triangles;

    int addVertex(const Vec3 &v)
    {
        vertices.push_back(v);
        return static_cast<int>(vertices.size()) - 1;
    }

    void addTriangle(int a, int b, int c)
    {
        if (a != b && b != c && a != c) {
            triangles.push_back({a, b, c});
        }
    }
};

struct SphereField {
    double radius;

    double value(const Vec3 &p) const
    {
        return dot(p, p) - radius * radius;
    }

    Vec3 gradient(const Vec3 &p) const
    {
        return normalize({2.0 * p.x, 2.0 * p.y, 2.0 * p.z});
    }
};

Vec3 lerpSurface(const Vec3 &a, const Vec3 &b, double va, double vb)
{
    const double denom = va - vb;
    double t = std::abs(denom) < 1e-12 ? 0.5 : va / denom;
    t = std::clamp(t, 0.0, 1.0);
    return a + (b - a) * t;
}

void orientAndAdd(Mesh &mesh, int a, int b, int c, const SphereField &field)
{
    const Vec3 p0 = mesh.vertices[a];
    const Vec3 p1 = mesh.vertices[b];
    const Vec3 p2 = mesh.vertices[c];
    const Vec3 normal = cross(p1 - p0, p2 - p0);
    const Vec3 center = (p0 + p1 + p2) / 3.0;

    if (dot(normal, field.gradient(center)) < 0.0) {
        mesh.addTriangle(a, c, b);
    } else {
        mesh.addTriangle(a, b, c);
    }
}

Vec3 gridPoint(int x, int y, int z, int resolution, double bounds)
{
    const double step = (2.0 * bounds) / resolution;
    return {
        -bounds + step * x,
        -bounds + step * y,
        -bounds + step * z
    };
}

Mesh marchingCubes(const SphereField &field, int resolution, double bounds)
{
    static const int cornerOffset[8][3] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
        {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1}
    };
    static const int edgeCorners[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
    static const int faces[6][4] = {
        {0, 1, 2, 3}, {4, 7, 6, 5}, {0, 4, 5, 1},
        {1, 5, 6, 2}, {2, 6, 7, 3}, {3, 7, 4, 0}
    };
    static const int faceEdges[6][4] = {
        {0, 1, 2, 3}, {7, 6, 5, 4}, {8, 4, 9, 0},
        {9, 5, 10, 1}, {10, 6, 11, 2}, {11, 7, 8, 3}
    };

    Mesh mesh;

    for (int z = 0; z < resolution; ++z) {
        for (int y = 0; y < resolution; ++y) {
            for (int x = 0; x < resolution; ++x) {
                std::array<Vec3, 8> p;
                std::array<double, 8> v;
                int insideCount = 0;

                for (int i = 0; i < 8; ++i) {
                    p[i] = gridPoint(x + cornerOffset[i][0], y + cornerOffset[i][1], z + cornerOffset[i][2], resolution, bounds);
                    v[i] = field.value(p[i]);
                    if (v[i] < 0.0) {
                        ++insideCount;
                    }
                }

                if (insideCount == 0 || insideCount == 8) {
                    continue;
                }

                std::array<int, 12> edgeVertex;
                edgeVertex.fill(-1);

                for (int e = 0; e < 12; ++e) {
                    const int a = edgeCorners[e][0];
                    const int b = edgeCorners[e][1];
                    if ((v[a] < 0.0) != (v[b] < 0.0)) {
                        edgeVertex[e] = mesh.addVertex(lerpSurface(p[a], p[b], v[a], v[b]));
                    }
                }

                std::vector<std::pair<int, int>> graph;
                for (int f = 0; f < 6; ++f) {
                    std::vector<int> crossing;
                    for (int e = 0; e < 4; ++e) {
                        if (edgeVertex[faceEdges[f][e]] >= 0) {
                            crossing.push_back(faceEdges[f][e]);
                        }
                    }

                    if (crossing.size() == 2) {
                        graph.push_back({crossing[0], crossing[1]});
                    } else if (crossing.size() == 4) {
                        const Vec3 center = (p[faces[f][0]] + p[faces[f][1]] + p[faces[f][2]] + p[faces[f][3]]) / 4.0;
                        const bool centerInside = field.value(center) < 0.0;
                        if (centerInside) {
                            graph.push_back({crossing[0], crossing[1]});
                            graph.push_back({crossing[2], crossing[3]});
                        } else {
                            graph.push_back({crossing[0], crossing[3]});
                            graph.push_back({crossing[1], crossing[2]});
                        }
                    }
                }

                std::array<std::vector<int>, 12> adjacency;
                for (const auto &edge : graph) {
                    adjacency[edge.first].push_back(edge.second);
                    adjacency[edge.second].push_back(edge.first);
                }

                std::array<bool, 12> used;
                used.fill(false);

                for (int start = 0; start < 12; ++start) {
                    if (edgeVertex[start] < 0 || used[start] || adjacency[start].empty()) {
                        continue;
                    }

                    std::vector<int> cycle;
                    int previous = -1;
                    int current = start;

                    for (int guard = 0; guard < 16; ++guard) {
                        cycle.push_back(edgeVertex[current]);
                        used[current] = true;

                        int next = -1;
                        for (int candidate : adjacency[current]) {
                            if (candidate != previous) {
                                next = candidate;
                                break;
                            }
                        }

                        if (next == -1 || next == start) {
                            break;
                        }

                        previous = current;
                        current = next;
                    }

                    if (cycle.size() >= 3) {
                        for (size_t i = 1; i + 1 < cycle.size(); ++i) {
                            orientAndAdd(mesh, cycle[0], cycle[i], cycle[i + 1], field);
                        }
                    }
                }
            }
        }
    }

    return mesh;
}

void writeObj(const std::string &path, const Mesh &mesh)
{
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Nao foi possivel escrever " + path);
    }

    out << std::fixed << std::setprecision(8);
    for (const Vec3 &v : mesh.vertices) {
        out << "v " << v.x << ' ' << v.y << ' ' << v.z << '\n';
    }
    for (const auto &t : mesh.triangles) {
        out << "f " << t[0] + 1 << ' ' << t[1] + 1 << ' ' << t[2] + 1 << '\n';
    }
}

int main(int argc, char **argv)
{
    try {
        int resolution = 64;
        double radius = 0.72;

        if (argc > 1) {
            resolution = std::stoi(argv[1]);
        }
        if (argc > 2) {
            radius = std::stod(argv[2]);
        }

        if (resolution < 4) {
            throw std::runtime_error("A resolucao do Marching Cubes deve ser pelo menos 4.");
        }
        if (radius <= 0.0 || radius >= 1.0) {
            throw std::runtime_error("Use um raio maior que 0 e menor que 1.");
        }

        const double bounds = 1.0;
        const SphereField sphere{radius};
        const Mesh mesh = marchingCubes(sphere, resolution, bounds);

        writeObj("sphere_marching_cubes.obj", mesh);

        std::cout << "Marching Cubes: " << mesh.vertices.size() << " vertices, "
                  << mesh.triangles.size() << " triangulos -> sphere_marching_cubes.obj\n";
    } catch (const std::exception &error) {
        std::cerr << "Erro: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
