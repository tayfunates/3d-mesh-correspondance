sourceShapePath = 'D:\tuff\data\FourLegsMeshes\shapes\384LowReso.off';
targetShapePath = 'D:\tuff\data\FourLegsMeshes\shapes\390LowReso.off';

[sourceVertexList, sourceFaceList] = read_off(sourceShapePath);
[targetVertexList, targetFaceList] = read_off(targetShapePath);

sourceModel.v = transpose(sourceVertexList); %Specify one vertex per row in V from patch.m
sourceModel.f = transpose(sourceFaceList);
%%% Create a yellow source model %%%
sourceModel.c = ones(size(sourceModel.v,1), 3);
sourceModel.c(:, 1) = sourceModel.c(:, 1) * 1.0;
sourceModel.c(:, 2) = sourceModel.c(:, 2) * 1.0;
sourceModel.c(:, 3) = sourceModel.c(:, 3) * 0.0;

fprintf('Displaying source Model...\n');
h = patch('Faces',sourceModel.f,'Vertices',sourceModel.v,'FaceVertexCData',sourceModel.c, 'FaceColor', 'interp');
hold on;
drawnow;

targetModel.v = transpose(targetVertexList);
targetModel.f = transpose(targetFaceList);
%%% Create a cyan target model %%%
targetModel.c = ones(size(targetModel.v,1), 3);
targetModel.c(:, 1) = targetModel.c(:, 1) * 0.0;
targetModel.c(:, 2) = targetModel.c(:, 2) * 1.0;
targetModel.c(:, 3) = targetModel.c(:, 3) * 1.0;

fprintf('Displaying target Model...\n');
h = patch('Faces',targetModel.f,'Vertices',targetModel.v,'FaceVertexCData',targetModel.c, 'FaceColor', 'interp');
hold on;
drawnow;


algorithm_type = 'dijkstra'; %% 'dijkstra', 'exact', 'subdivision'

%%% Calculate the pairwise distances in the source model %%%
[D0] = pairwiseGeodesicDistances(sourceModel, algorithm_type);

%%% Calculate the pairwise distances in the target model %%%
[D] = pairwiseGeodesicDistances(targetModel, algorithm_type);

n0 = size(D0,1); % Size of metric matrices
n = size(D,1);

%%%% Create the options if necessary %%%

%options.mu0 = ones(n0,1)/n0; % Measure on the source domain
%options.mu =  ones(n,1)/n; % Measure on the target domain
%options.regularizer = .005; %alpha
%options.initialRegularizer = regularizer; % for multiscale
%options.regularizerChangeRate = 1; % how fast to approach alpha
%options.maxIter = 40; % Maximum number of GW iterations
options.display = 0; % Show map matrix in each iteration
%options.relativeRegularizer = 0; % Alpha relative to matrix
%options.GWTol = 1e-7; % Tolerance for Gromov-Wasserstein iteration
%options.sinkhornTol = 1e-6; % Tolerance for Sinkhorn iteration
%options.maxSinkhornIter = 500; % Max inner loop
%options.toSubtract = sparse(n0,n); % Experimental for symmetry detection
%options.normalizeDistances = 1; % Divide distances by max
%options.plotObjective = 0; % Plot objective in each iteration
%options.redrawFrequency = 1;
%options.initialGuess = ones(n0,n); % Initial guess of matching
%options.eta = 1;
%options.stencil = ones(n0,n);
%options.partialSource = 0;
%options.partialTarget = 0;

[gamma, objectives] = gromovWassersteinDistance(D0, D, options);

%%% identify a couple of source vertices
sourceVertexIndices = [1, 4, 10, 13, 60, 340];
sourceVertexColors = [];
 
for i = 1:length(sourceVertexIndices)
    randColor = [rand() rand() rand()]; 
    sourceModel.c(sourceVertexIndices(i),:) = randColor;

    [maxVal maxIdx] = max(gamma(sourceVertexIndices(i),:));
    targetModel.c(maxIdx,:) = randColor;

    sourceVertex = sourceModel.v(sourceVertexIndices(i),:);
    targetVertex = targetModel.v(maxIdx,:);
    
    r=0.03;
    phi=linspace(0,pi,5);
    theta=linspace(0,2*pi,10);
    [phi,theta]=meshgrid(phi,theta);
    x=r*sin(phi).*cos(theta);
    y=r*sin(phi).*sin(theta);
    z=r*cos(phi);
    surface = surf(x+sourceVertex(1),y+sourceVertex(2),z+sourceVertex(3));
    hold on;  
    set(surface,'FaceColor',randColor, 'FaceAlpha',1.0,'FaceLighting','gouraud','EdgeColor','none')
    
    r=0.03;
    phi=linspace(0,pi,5);
    theta=linspace(0,2*pi,10);  
    [phi,theta]=meshgrid(phi,theta);
    x=r*sin(phi).*cos(theta);
    y=r*sin(phi).*sin(theta);
    z=r*cos(phi); 
    surface = surf(x+targetVertex(1),y+targetVertex(2),z+targetVertex(3));
    hold on;
    set(surface,'FaceColor',randColor, 'FaceAlpha',1.0,'FaceLighting','gouraud','EdgeColor','none')
end

;


