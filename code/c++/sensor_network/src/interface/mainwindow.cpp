#include "mainwindow.h"

MainWindow::MainWindow(){
    data_set = new DataSet(0,0,0,vector<pair<float, float> >());
    Solution* empty_solution = new Solution(data_set);
    data_set->addSolution(empty_solution);

    targets_table_model = new TargetsTableModel(data_set, this);
    parameters_model = new ParametersModel(data_set, this);
    solutions_table_model = new SolutionsTableModel(data_set, this);
    solution_model = new SolutionModel(empty_solution, this);

    sidebar = new Sidebar(targets_table_model, parameters_model, solutions_table_model, this);
    sidebar->setAllowedAreas(Qt::LeftDockWidgetArea);
    sidebar->setFloating(false);
    addDockWidget(Qt::LeftDockWidgetArea, sidebar);

    central_widget = new CentralWidget(solution_model, this);
    setCentralWidget(central_widget);


    // building menu bar
    QMenu *file_menu = menuBar()->addMenu("File");
    file_menu->addAction("Load data set", this, SLOT(loadDataSet()));
    QMenu *view_menu = menuBar()->addMenu("View");
    view_menu->addAction(sidebar->toggleViewAction());

    connect(sidebar, &Sidebar::selectedSolutionChanged, this, &MainWindow::handleSelectedSolutionChanged);
}

MainWindow::~MainWindow(){
    delete data_set;
}

void MainWindow::closeEvent(QCloseEvent *event){
    int answer = QMessageBox::warning(this, "Quit?", "Are you sure you want to quit?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(answer == QMessageBox::Yes){
        event->accept();
    } else{
        event->ignore();
    }
}

void MainWindow::loadDataSet(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString file_name = QFileDialog::getOpenFileName(nullptr, "Select a file to open.", QString());//, "Text files (*.txt);;XML files (*.xml);;");
    // !!! catch errors durind loading
    if(file_name.length()!=0){
        DataSet* new_data_set = new DataSet(2, 2, 2, parseCoordinates(file_name.toStdString()));

        int number_targets = new_data_set->getNumberTargets();
        int reception_level = new_data_set->getReceptionLevel();

        vector<int> seed_vector = vector<int>(reception_level*number_targets, 0);
        for(int target_index = 0; target_index<number_targets; target_index++){
            for(int reception = 0; reception<reception_level; reception++){
                seed_vector[target_index*reception_level+reception] = target_index;
            }
        }
        vector<int> order_vector = vector<int>(number_targets, 0);
        for(int target_index = 0; target_index<number_targets; target_index++){
            order_vector[target_index] = target_index;
        }

        for(int i = 0; i<1; i++){
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle(seed_vector.begin(), seed_vector.end(), std::default_random_engine(seed));
            Solution* heuristic_solution = shortestPathsHeuristic(new_data_set, seed_vector);
            Solution* metaheuristic_solution = seedVectorMetaheuristic(10000, seed_vector, new_data_set);//receptionOrientedMetaheuristic(1000, heuristic_solution);
            new_data_set->addSolution(heuristic_solution);
            new_data_set->addSolution(metaheuristic_solution);
        }

        setDataSet(new_data_set);
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::setDataSet(DataSet *new_data_set){
    delete data_set;

    if(!new_data_set->getNumberSolutions()){
        Solution* empty_solution = new Solution(new_data_set);
        new_data_set->addSolution(empty_solution);
    }

    data_set = new_data_set;

    targets_table_model->setDataSet(new_data_set);
    parameters_model->setDataSet(new_data_set);
    solutions_table_model->setDataSet(new_data_set);
    solution_model->setSolution(new_data_set->getSolution(0));
}

void MainWindow::handleSelectedSolutionChanged(int solution_index){
    solution_model->setSolution(data_set->getSolution(solution_index));
}
