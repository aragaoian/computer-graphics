# M3 - Marching Cubes

Gera uma malha OBJ para uma esfera implicita:

- `sphere_marching_cubes.obj`

## Executar

```bash
make
./m3
```

Parametros opcionais:

```bash
./m3 <resolucao> <raio>
```

Exemplo:

```bash
./m3 64 0.72
```

O Marching Cubes percorre uma grade regular, encontra as intersecoes da esfera implicita com as arestas de cada cubo e triangula os ciclos de intersecao para gerar a superficie.
