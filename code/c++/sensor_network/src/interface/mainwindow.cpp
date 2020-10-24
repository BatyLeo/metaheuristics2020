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

    current_data_set_folder_path = QString();


    // building menu bar
    QMenu *file_menu = menuBar()->addMenu("File");
    file_menu->addAction("Load data set", this, SLOT(loadDataSet()));
    file_menu->addAction("Load saved data set", this, SLOT(loadSavedDataSet()));
    file_menu->addAction("Save data set as", this, SLOT(saveDataSetAs()));
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
    QString file_name = QFileDialog::getOpenFileName(this, "Select a file to open.", QString());//, "Text files (*.txt);;XML files (*.xml);;");
    // !!! catch errors durind loading
    if(file_name.length()!=0){
        DataSet* new_data_set = new DataSet(2, 1, 1, parseCoordinates(file_name.toStdString()));

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
            shuffle(seed_vector.begin(), seed_vector.end(), std::default_random_engine(0));
            Solution* heuristic_solution = shortestPathsHeuristic(new_data_set, seed_vector);
            Solution* metaheuristic_solution = simulatedAnnealingMetaheuristic(2000, heuristic_solution, 10000, 0.9, 0.01);//seedVectorMetaheuristic(10000, seed_vector, new_data_set);//receptionOrientedMetaheuristic(1000, heuristic_solution);
            new_data_set->addSolution(heuristic_solution);
            new_data_set->addSolution(metaheuristic_solution);
            if(!metaheuristic_solution->checkAdmissible()){
                cout<<"dommage..."<<endl;
            }
        }

        setDataSet(new_data_set);
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::loadSavedDataSet(){
    QString folder_name = QFileDialog::getExistingDirectory(this, "Choose a project folder");

    QString data_set_file_name = folder_name+QString("/data_set");
    if(!QFile::exists(data_set_file_name)){
        QMessageBox::warning(this, "File not found", "The file containing the data set could not be found", QMessageBox::Ok, QMessageBox::Ok);
    } else{
        vector<pair<float, float> > coordinates = parseCoordinates(data_set_file_name.toStdString());
        DataSet *saved_data_set = new DataSet(2, 1, 1, coordinates);

        QString solutions_folder_name = folder_name+QString("/solutions");

        int solution_index = 0;
        bool solution_exists = true;
        while(solution_exists){
            QString solution_file_name = solutions_folder_name+QString("/solution_%1").arg(solution_index);
            solution_exists = QFile::exists(solution_file_name);
            if(solution_exists){
                vector<bool> sensor_placement = parseSensorPlacement(solution_file_name.toStdString(), saved_data_set->getNumberTargets());
                Solution *solution = new Solution(saved_data_set, sensor_placement);
                saved_data_set->addSolution(solution);
            }
            solution_index++;
        }
        current_data_set_folder_path = folder_name;
        setDataSet(saved_data_set);
    }
}

void MainWindow::saveDataSetAs(){
    if(!data_set){
        return;
    }

    QString folder_name = QFileDialog::getSaveFileName(this, "Save file as");

    if(folder_name.length()==0){
        QMessageBox::warning(this, "Empty file name", "The data set could not be saved with an empty file name", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QDir().mkdir(folder_name);
    writeDataSetToFile(folder_name.toStdString(), "data_set", data_set);

    QString solutions_folder_name = folder_name+QString("/solutions");
    QDir().mkdir(solutions_folder_name);

    int number_solutions = data_set->getNumberSolutions();
    for(int solution_index = 0; solution_index<number_solutions; solution_index++){
        Solution *solution = data_set->getSolution(solution_index);
        writeSolutionToFile(solutions_folder_name.toStdString(), "solution_"+to_string(solution_index), solution);
    }
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
