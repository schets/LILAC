a = load('iso_out.out');
[X, Y, Z] = meshgrid(unique(a(:, 1)), unique(a(:, 2)), unique(a(:, 3)));
V = zeros(length(Y), length(Y), length(Z));
for i = 1:numel(V)
  idx = find(a(:, 1) == X(i) & a(:, 2) == Y(i) & a(:, 3) == Z(i));
  V(i) = a(idx, 4);
end
fv = isosurface (X,Y,Z,V,0.9);
p = patch(fv);
set(p,'FaceColor','red','EdgeColor','none');
camlight;
lighting gouraud;
xlabel('x');
ylabel('y');
zlabel('z');
