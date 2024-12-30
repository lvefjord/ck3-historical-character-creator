#include "mainwindow.h"
#include "charactertoken.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QRegularExpression"
#include "QScrollBar"
#include "characterdata.h"
#include "QValidator"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentFile = "";
    setWindowTitle(tr("Untitled"));
    // date validator stuff
    QRegularExpression re("^\\d+\\.(1[0-2]|[1-9])\\.(30|[12]\\d|[1-9])$");
    QRegularExpressionValidator *dateValidator = new QRegularExpressionValidator(re, this);
    // for random example character generation
    srand(static_cast<unsigned int>(time(0)));

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeFile);
    connect(ui->actionReload_File, &QAction::triggered, this, &MainWindow::reloadFile);
    connect(ui->nameField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    connect(ui->dnaField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    connect(ui->religionField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    connect(ui->cultureField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    connect(ui->dynField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    connect(ui->bdayField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    ui->bdayField->setValidator(dateValidator);
    connect(ui->deathdayField, &QLineEdit::textEdited, this, &MainWindow::on_FieldEdited);
    ui->deathdayField->setValidator(dateValidator);

    // family tree stuff
    familyTreeScene = new QGraphicsScene(this);
    ui->familyTreeView->setScene(familyTreeScene);

    initializeCharacterSheet();


    // trait buttons
    QList<QPushButton*> traitButtons = ui->traitTabs->findChildren<QPushButton*>();
    for (QPushButton *button : traitButtons)
    {
        connect(button, &QPushButton::clicked, this, &MainWindow::on_traitButton_clicked);
    }
    // setup trait tabs
    QSizePolicy sp_retain = ui->traitTabs->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->traitTabs->setSizePolicy(sp_retain);
    ui->traitTabs->setVisible(false);
    // setup agot traits
    ui->inquisitive->setVisible(false);
    ui->authoritative->setVisible(false);
    ui->rude->setVisible(false);
}

void MainWindow::on_removeSpousesRequested(QString characterId)
{
    if (!charactersById.contains(characterId)) return;
    CharacterData *cd = charactersById[characterId];

    // remove all spouse connections
    for (const QString &spouseId : cd->spouseIds) {
        if (charactersById.contains(spouseId)) {
            CharacterData *spouse = charactersById[spouseId];
            spouse->spouseIds.removeAll(characterId);
        }
    }
    cd->spouseIds.clear();
}

void MainWindow::initializeCharacterSheet()
{
    characters.clear();
    charactersById.clear();
    tokensById.clear();
    parentLines.clear();
    familyTreeScene->clear();

    CharacterData *initialCharacter = new CharacterData();
    exampleCharacterSelection(initialCharacter);
    charactersById[initialCharacter->id] = initialCharacter;
    addCharacterToScene(initialCharacter);
    updateAllCharacterText();
}

void MainWindow::exampleCharacterSelection(CharacterData *initialCharacter)
{
    int randNum = rand() % 5;
    if (randNum == 1){
        ui->nameField->setText("Henry");
        ui->dynField->setText("normandy");
        ui->religionField->setText("catholic");
        ui->cultureField->setText("english");
        if (!ui->actionToggle_DNA_field->isChecked()) ui->dnaField->setText("normandy_1");
        ui->bdayField->setText("1068.8.20");
        if (!ui->actionToggle_Death_date_field->isChecked()) ui->deathdayField->setText("1135.12.1");


        initialCharacter->characterNumber = 1;
        initialCharacter->id = "character_" + QString::number(initialCharacter->characterNumber);
        initialCharacter->name = "Henry";
        initialCharacter->dynasty = "normandy";
        initialCharacter->religion = "catholic";
        initialCharacter->culture = "english";
        if (!ui->actionToggle_DNA_field->isChecked()) initialCharacter->dna = "normandy_1";
        initialCharacter->birth = "1068.8.20";
        if (!ui->actionToggle_Death_date_field->isChecked()) initialCharacter->death = "1135.12.1";
        characters.append(initialCharacter);
        updateAllCharacterText();
    }
    else if (randNum == 2){
        ui->nameField->setText("Mahaut");
        ui->dynField->setText("artois");
        ui->religionField->setText("catholic");
        ui->cultureField->setText("french");
        if (!ui->actionToggle_DNA_field->isChecked()) ui->dnaField->setText("artois_1");
        ui->bdayField->setText("1268.3.20");
        if (!ui->actionToggle_Death_date_field->isChecked()) ui->deathdayField->setText("1329.11.27");
        ui->female_checkBox->setChecked(true);


        initialCharacter->characterNumber = 1;
        initialCharacter->id = "character_" + QString::number(initialCharacter->characterNumber);
        initialCharacter->name = "Mahaut";
        initialCharacter->dynasty = "artois";
        initialCharacter->religion = "catholic";
        initialCharacter->culture = "french";
        initialCharacter->gender = "female";
        if (!ui->actionToggle_DNA_field->isChecked()) initialCharacter->dna = "artois_1";
        initialCharacter->birth = "1268.3.20";
        if (!ui->actionToggle_Death_date_field->isChecked()) initialCharacter->death = "1329.11.27";
        characters.append(initialCharacter);
        updateAllCharacterText();
    }
    else if (randNum == 3){
        ui->nameField->setText("Philippe");
        ui->dynField->setText("capet");
        ui->religionField->setText("catholic");
        ui->cultureField->setText("french");
        if (!ui->actionToggle_DNA_field->isChecked()) ui->dnaField->setText("capet_1");
        ui->bdayField->setText("1268.4.4");
        if (!ui->actionToggle_Death_date_field->isChecked()) ui->deathdayField->setText("1314.11.29");


        initialCharacter->characterNumber = 1;
        initialCharacter->id = "character_" + QString::number(initialCharacter->characterNumber);
        initialCharacter->name = "Philippe";
        initialCharacter->dynasty = "capet";
        initialCharacter->religion = "catholic";
        initialCharacter->culture = "french";
        if (!ui->actionToggle_DNA_field->isChecked()) initialCharacter->dna = "capet_1";
        initialCharacter->birth = "1268.4.4";
        if (!ui->actionToggle_Death_date_field->isChecked()) initialCharacter->death = "1314.11.29";
        characters.append(initialCharacter);
        updateAllCharacterText();
    }
    else if (randNum == 4){
        ui->nameField->setText("Dante");
        ui->dynField->setText("alighieri");
        ui->religionField->setText("catholic");
        ui->cultureField->setText("italian");
        if (!ui->actionToggle_DNA_field->isChecked()) ui->dnaField->setText("alighieri_1");
        ui->bdayField->setText("1265.3.17");
        if (!ui->actionToggle_Death_date_field->isChecked()) ui->deathdayField->setText("1321.9.14");


        initialCharacter->characterNumber = 1;
        initialCharacter->id = "character_" + QString::number(initialCharacter->characterNumber);
        initialCharacter->name = "Dante";
        initialCharacter->dynasty = "alighieri";
        initialCharacter->religion = "catholic";
        initialCharacter->culture = "italian";
        if (!ui->actionToggle_DNA_field->isChecked()) initialCharacter->dna = "alighieri_1";
        initialCharacter->birth = "1265.3.17";
        if (!ui->actionToggle_Death_date_field->isChecked()) initialCharacter->death = "1321.9.14";
        characters.append(initialCharacter);
        updateAllCharacterText();
    }
    else if (randNum == 0){
        ui->nameField->setText("Gustav");
        ui->dynField->setText("vasa");
        ui->religionField->setText("protestant");
        ui->cultureField->setText("swedish");
        if (!ui->actionToggle_DNA_field->isChecked()) ui->dnaField->setText("vasa_1");
        ui->bdayField->setText("1594.12.9");
        if (!ui->actionToggle_Death_date_field->isChecked()) ui->deathdayField->setText("1632.11.6");


        initialCharacter->characterNumber = 1;
        initialCharacter->id = "character_" + QString::number(initialCharacter->characterNumber);
        initialCharacter->name = "Gustav";
        initialCharacter->dynasty = "vasa";
        initialCharacter->religion = "protestant";
        initialCharacter->culture = "swedish";
        if (!ui->actionToggle_DNA_field->isChecked()) initialCharacter->dna = "vasa_1";
        initialCharacter->birth = "1594.12.9";
        if (!ui->actionToggle_Death_date_field->isChecked()) initialCharacter->death = "1632.11.6";
        characters.append(initialCharacter);
        updateAllCharacterText();
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            ui->openFileEdit->setPlainText(in.readAll());
            file.close();
            currentFile = fileName;
            setWindowTitle(fileName);
        }
    }
}
void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << ui->openFileEdit->toPlainText();
            file.close();
            ui->openFileEdit->document()->setModified(false);
            setWindowTitle(currentFile);
        }
    }
}
void MainWindow::saveFile()
{
    if (currentFile.isEmpty())
    {
        saveFileAs();
    }
    else
    {
        QFile file(currentFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << ui->openFileEdit->toPlainText();
            file.close();
            ui->openFileEdit->document()->setModified(false);
            setWindowTitle(currentFile);
        }
    }
}
void MainWindow::closeFile()
{
    if (ui->openFileEdit->document()->isModified())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("Unsaved Changes"),
            tr("You have unsaved changes. Do you want to save them before closing?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

        if (reply == QMessageBox::Yes)
        {
            saveFile();
        }
        else if (reply == QMessageBox::Cancel)
        {
            return;
        }
    }

    ui->openFileEdit->clear();
    currentFile = "";
    ui->openFileEdit->document()->setModified(false);
    setWindowTitle(tr("Untitled"));
}
void MainWindow::reloadFile()
{
    if (ui->openFileEdit->document()->isModified())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            tr("Unsaved Changes"),
            tr("You have unsaved changes. Do you want to save them before closing?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Yes)
        {
            saveFile();
        }
        else if (reply == QMessageBox::Cancel)
        {
            return;
        }
    }
    if (!currentFile.isEmpty())
    {
        QFile file(currentFile);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            ui->openFileEdit->setPlainText(in.readAll());
            file.close();
            ui->openFileEdit->document()->setModified(false);
            setWindowTitle(currentFile);
            statusBar()->showMessage(tr("File reloaded successfully"), 3000);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::formatCharacter(CharacterData *character, int characterIndex)
{
    QString formattedText;

    if (!character->dynasty.isEmpty())
    {
        formattedText += QString("%1_%2 = {\n").arg(character->dynasty).arg(characterIndex);
    }
    else
    {
        formattedText += QString("character_%1 = {\n").arg(character->dynasty);
    }

    if (!character->name.isEmpty())
        formattedText += QString ("\tname = \%1\n").arg(character->name);
    if (!character->dna.isEmpty())
        formattedText += QString ("\tdna = \%1\n").arg(character->dna);
    if (!character->dynasty.isEmpty())
        formattedText += QString("\tdynasty = dynn_\%1\n").arg(character->dynasty);
    if (!character->culture.isEmpty())
        formattedText += QString ("\tculture = \%1\n").arg(character->culture);
    if (!character->religion.isEmpty())
        formattedText += QString ("\treligion = \%1\n").arg(character->religion);

    if (character->gender == "female")
        formattedText += QString ("\tfemale = yes\n");

    // rebuild the dynasty names | because I started using stable ids instead of dynamic ones
    CharacterData *fatherData = nullptr;
    CharacterData *motherData = nullptr;
    auto buildDynastyName = [&](CharacterData *c) {
        if (!c->dynasty.isEmpty())
            return c->dynasty + "_" + QString::number(c->characterNumber);
        else
            return QString("character_") + QString::number(c->characterNumber);
    };

    if (!character->fatherId.isEmpty() && charactersById.contains(character->fatherId)){
        fatherData = charactersById[character->fatherId];
        if (fatherData) {
            QString fatherLabel = buildDynastyName(fatherData);
            formattedText += QString ("\tfather = \%1\n").arg(fatherLabel);
        }
    }

    if (!character->motherId.isEmpty() && charactersById.contains(character->motherId)){
        motherData = charactersById[character->motherId];
        if (motherData) {
            QString motherLabel = buildDynastyName(motherData);
            formattedText += QString ("\tmother = \%1\n").arg(motherLabel);
        }
    }

    if (character->disallowRandomTraits == true)
    {
        formattedText += QString("\tdisallow_random_traits = yes\n");
    }

    for (const QString &trait : character->traits)
    {
        if (trait.contains("i_e_")){
            QStringList traitParts = trait.split('_');
            QString updatedTrait = "education_intrigue_" + traitParts[2];
            formattedText += QString("\ttrait = %1\n").arg(updatedTrait);
        }
        else if (trait.contains("d_e_")){
            QStringList traitParts = trait.split('_');
            QString updatedTrait = "education_diplomacy_" + traitParts[2];
            formattedText += QString("\ttrait = %1\n").arg(updatedTrait);
        }
        else if (trait.contains("s_e_")){
            QStringList traitParts = trait.split('_');
            QString updatedTrait = "education_stewardship_" + traitParts[2];
            formattedText += QString("\ttrait = %1\n").arg(updatedTrait);
        }
        else if (trait.contains("m_e_")){
            QStringList traitParts = trait.split('_');
            QString updatedTrait = "education_martial_" + traitParts[2];
            formattedText += QString("\ttrait = %1\n").arg(updatedTrait);
        }
        else if (trait.contains("l_e_")){
            QStringList traitParts = trait.split('_');
            QString updatedTrait = "education_learning_" + traitParts[2];
            formattedText += QString("\ttrait = %1\n").arg(updatedTrait);
        }
        else if (trait.contains("e_m_p_")) {
            QStringList traitParts = trait.split('_');
            QString updatedTrait = "education_martial_prowess_" + traitParts[3];
            formattedText += QString("\ttrait = %1\n").arg(updatedTrait);
        }
        else
            formattedText += QString("\ttrait = %1\n").arg(trait);
    }

    if (!character->birth.isEmpty())
        formattedText += QString ("\t%1 = { birth = yes }\n").arg(character->birth);

    for (const QString &spouseId : character->spouseIds)
    {
        if (!charactersById.contains(spouseId)) continue;

        CharacterData *spouseData = charactersById[spouseId];

        QString spouseLabel = buildDynastyName(spouseData);

        QStringList birthParts = character->birth.split('.');
        int birthyearPlus16 = 0;
        //QString earliestMarryDate = "x.x.x";
        if (birthParts.size() == 3){
            int birthYear = birthParts[0].toInt();
            birthyearPlus16 = birthYear + 16;
        }
        QString earliestMarryDate = QString::number(birthyearPlus16) + "." + birthParts[1] + "." + birthParts[2];
        formattedText += QString("\t%2 = { add_spouse = %1 }\n").arg(spouseLabel).arg(earliestMarryDate);
    }

    if (!character->death.isEmpty())
        formattedText += QString ("\t%1 = { death = yes }\n").arg(character->death);


    formattedText += "}\n\n";
    return formattedText;
}

void MainWindow::on_FieldEdited(const QString &text)
{
    //CharacterData *activeChar = characters.last();
    if (characters.isEmpty())
        return;

    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender());
    if (!lineEdit)
        return;

    QString key;
    if (lineEdit == ui->nameField)
        key = "name";
    else if (lineEdit == ui->dnaField)
        key = "dna";
    else if (lineEdit == ui->religionField)
        key = "religion";
    else if (lineEdit == ui->cultureField)
        key = "culture";
    else if (lineEdit == ui->dynField)
        key = "dynasty";
    else if (lineEdit == ui->bdayField)
        key = "birth";
    else if (lineEdit == ui->deathdayField)
        key = "death";

    if (key.isEmpty())
        return;

    CharacterData *activeChar = characters.last();
    if (key == "name")
        activeChar->name = text;
    else if (key == "dna")
        activeChar->dna = text;
    else if (key == "religion")
        activeChar->religion = text;
    else if (key == "culture")
        activeChar->culture = text;
    else if (key == "dynasty")
        activeChar->dynasty = text;
    else if (key == "birth")
        activeChar->birth = text;
    else if (key == "death")
        activeChar->death = text;

    // if dna checkbox is checked updated DNA
    updateDnaIfChecked(activeChar);

    updateAllCharacterText();
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
}

void MainWindow::on_traitButton_clicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button || characters.isEmpty())
        return;

    QString traitName = button->objectName();
    if (traitName.isEmpty())
        return;

    CharacterData *activeCharacter = characters.last();

    QStringList traits = activeCharacter->traits;

    if (button->isChecked())
    {
        if (!traits.contains(traitName))
            traits.append(traitName);
    }
    else
    {
        traits.removeAll(traitName);
    }

    activeCharacter->traits = traits;

    updateAllCharacterText();
    ui->openFileEdit_newchars->ensureCursorVisible();
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
}

void MainWindow::on_addCharacter_clicked()
{
    if (characters.isEmpty())
        return;

    CharacterData *newCharacter = new CharacterData();
    int maxNumber = 0;
    for (CharacterData *c : characters) {
        if (c->characterNumber > maxNumber) {
            maxNumber = c->characterNumber;
        }
    }
    newCharacter->characterNumber = maxNumber + 1;

    newCharacter->name = ui->nameField->text();
    newCharacter->dna = ui->dnaField->text();
    newCharacter->fatherId = "";
    newCharacter->motherId = "";
    newCharacter->religion = ui->religionField->text();
    newCharacter->culture = ui->cultureField->text();
    newCharacter->dynasty = ui->dynField->text();
    newCharacter->birth = ui->bdayField->text();
    newCharacter->death = ui->deathdayField->text();

    if (!newCharacter->dynasty.isEmpty())
        newCharacter->id = newCharacter->dynasty + "_" + QString::number(newCharacter->characterNumber);
    else
        newCharacter->id = "character_" + QString::number(newCharacter->characterNumber);

    characters.append(newCharacter);
    charactersById[newCharacter->id] = newCharacter;
    addCharacterToScene(newCharacter);
    updateDnaIfChecked(newCharacter);
    updateAllCharacterText();
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
    ui->female_checkBox->setChecked(false);
    ui->rtraits_checkBox->setChecked(false);
    // uncheck trait buttons
    QList<QPushButton*> traitButtons = ui->traitTabs->findChildren<QPushButton*>();
    for (QPushButton *button : traitButtons)
    {
        if (button->isChecked()) button->setChecked(false);
    }
}

void MainWindow::on_female_checkBox_stateChanged(int state)
{
    if (characters.isEmpty())
        return;

    CharacterData *activeCharacter = characters.last();

    if (state == Qt::Checked)
    {
        activeCharacter->gender = "female";
    }
    else
    {
        activeCharacter->gender = "";
    }

    updateAllCharacterText();
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
}


void MainWindow::on_rtraits_checkBox_stateChanged(int state)
{
    if (characters.isEmpty())
        return;

    CharacterData *activeCharacter = characters.last();

    if (state == Qt::Checked)
    {
        activeCharacter->disallowRandomTraits = true;
        ui->traitTabs->setVisible(true);
    }
    else
    {
        activeCharacter->disallowRandomTraits = false;
        ui->traitTabs->setVisible(false);
    }

    updateAllCharacterText();
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
}
void MainWindow::updateAllCharacterText()
{
    QString updatedText;
    for (int i = 0; i < characters.size(); ++i)
    {
        updatedText += formatCharacter(characters[i], characters[i]->characterNumber);
    }
    ui->openFileEdit_newchars->setPlainText(updatedText);
}

void MainWindow::addCharacterToScene(CharacterData *character)
{
    CharacterToken *token = new CharacterToken(character);
    // connect token signal to slot in MainWindow
    connect(token, &CharacterToken::relationshipsChanged, this, &MainWindow::updateAllCharacterText);
    connect(token, &CharacterToken::spousesSet, this, &MainWindow::drawSpouseLine);
    connect(token, &CharacterToken::parentSet, this, &MainWindow::drawParentLine);
    connect(token, &CharacterToken::tokenMoved, this, &MainWindow::updateAllLines);
    connect(token, &CharacterToken::removeSpousesRequested, this, &MainWindow::on_removeSpousesRequested);

    // start at base position
    int x = 0, y = 0;
    token->setPos(x, y);
    familyTreeScene->addItem(token);

    while (!token->collidingItems().isEmpty()) {
        x += 20;
        token->setPos(x, y);

        if (x > 500){
            x = 0;
            y += 80;
            token->setPos(x, y);
        }
    }

    tokensById[character->id] = token;
}

void MainWindow::drawParentLine(QString childId, QString parentId)
{
    // make sure both exist
    if (!tokensById.contains(childId) || !tokensById.contains(parentId)){
        qDebug() << "one of the tokens is missing / drawParentLine";
        return;
    }

    CharacterToken *childToken = tokensById[childId];
    CharacterToken *parentToken = tokensById[parentId];

    // get child's CharacterData and parent(s)
    CharacterData *childData = charactersById[childId];
    QString fatherId = childData->fatherId;
    QString motherId = childData->motherId;

    // decide where line starts from
    QPointF startPoint;
    if (!fatherId.isEmpty() && !motherId.isEmpty()){
        // two parents known check if spouse
        if (!tokensById.contains(fatherId) || !tokensById.contains(motherId))
            return;

        CharacterToken *fatherToken = tokensById[fatherId];
        CharacterToken *motherToken = tokensById[motherId];

        QPointF fatherPos = fatherToken->scenePos();
        QPointF motherPos = motherToken->scenePos();
        startPoint = QPointF((fatherPos.x() + motherPos.x())/2 + 50, (fatherPos.y() + motherPos.y())/2 + 25);
    }
    else {
        // only one parent known
        QPointF parentPos = parentToken->scenePos();
        QRectF parentRect = parentToken->boundingRect();
        startPoint = parentPos + QPointF(parentRect.width()/2, parentRect.height());
    }

    // end point is childs top center
    QRectF childRect = childToken->boundingRect();
    QPointF endPoint = childToken->scenePos() + QPointF(childRect.width()/2, 0);

    // create a path
    QPainterPath path(startPoint);
    qreal midY = (startPoint.y() + endPoint.y()) / 2;
    path.lineTo(startPoint.x(), midY); // down from parent midpoint
    path.lineTo(endPoint.x(), midY);   // horizontal line over child's x
    path.lineTo(endPoint);            // down to child's top

    // if a line already exists for this child (might be redrawing)
    if (parentLines.contains(childId)) {
        QGraphicsPathItem *lineItem = parentLines[childId];
        lineItem->setPath(path);
    } else {
        QGraphicsPathItem *lineItem = familyTreeScene->addPath(path, QPen(Qt::white, 3));
        parentLines[childId] = lineItem;
    }
}

void MainWindow::drawSpouseLine(QString char1Id, QString char2Id)
{
    if (!tokensById.contains(char1Id) || !tokensById.contains(char2Id))
    {
        return;
    }

    CharacterToken *token1 = tokensById[char1Id];
    CharacterToken *token2 = tokensById[char2Id];

    // get positions
    QPointF pos2 = token2->scenePos();
    //token1->setPos(pos2.x() - 120, pos2.y()); // align tokens for line
    QPointF pos1 = token1->scenePos();

    //QPointF startPoint;
    QRectF spouse1Rect = token1->boundingRect();
    //QPointF pos1 = token1->scenePos();
    QPointF startPoint = pos1 + QPointF(spouse1Rect.width(), spouse1Rect.height()/2);
    //QPointF pos2 = token2->scenePos();
    QRectF spouse2Rect = token2->boundingRect();
    QPointF endPoint = pos2 + QPointF(spouse2Rect.width()*0, spouse2Rect.height()/2);

    qreal startX = startPoint.x();
    qreal startY = startPoint.y();
    qreal endX = endPoint.x();
    qreal endY = endPoint.y();

    qreal midX = (startX + endX) / 2;

    // create path
    QPainterPath path(startPoint);
    // fuck this math shit i have no idea but it works 💀
    path.lineTo(midX, startY);

    path.lineTo(midX, endY);

    path.lineTo(endX, endY);

    if (spouseLines.contains(char1Id)) {
        QGraphicsPathItem *lineItem = spouseLines[char1Id];
        lineItem->setPath(path);
    } else {
        QGraphicsPathItem *lineItem = familyTreeScene->addPath(path, QPen(Qt::white, 3));
        spouseLines[char1Id] = lineItem;
    }
}

void MainWindow::updateAllLines()
{
    // for each child that has a parent line, recalculate the the path

    for (auto it = parentLines.begin(); it != parentLines.end(); ++it){
        QString childId = it.key();
        CharacterData *childData = charactersById[childId];
        if (!childData) continue;
        // get parentId(s)
        QString fatherId = childData->fatherId;
        QString motherId = childData->motherId;

        // call drawparentline again if father or mother exists
        if (!fatherId.isEmpty() && motherId.isEmpty()){
            drawParentLine(childId, fatherId);
        } else if (!motherId.isEmpty() && fatherId.isEmpty()){
            drawParentLine(childId, motherId);
        } else if (!fatherId.isEmpty() && !motherId.isEmpty()) {
            drawParentLine(childId, fatherId);
        }
    }
    // for spouse lines
    for (auto it = spouseLines.begin(); it != spouseLines.end(); ++it) {
        QString spouseKey = it.key();
        if (!charactersById.contains(spouseKey)) continue; // safety check
        CharacterData *cdata = charactersById[spouseKey];
        // only 1 spouse per for now
        for (const QString &spouseId : cdata->spouseIds) {
            drawSpouseLine(spouseKey, spouseId);
        }
    }
}



void MainWindow::on_dna_checkBox_checkStateChanged(const Qt::CheckState &state)
{
    if (characters.isEmpty()) return;

    CharacterData *activeCharacter = characters.last();

    ui->dnaField->setReadOnly(state == Qt::Checked);

    updateDnaIfChecked(activeCharacter);

    updateAllCharacterText();
}

void MainWindow::updateDnaIfChecked(CharacterData *c)
{
    if (!c) return;

    if (ui->dna_checkBox->isChecked()) {
        int charIndex = c->characterNumber;
        QString newDna;
        if (!c->dynasty.isEmpty()) {
            newDna = c->dynasty + "_" + QString::number(charIndex);
        } else {
            newDna = "character_" + QString::number(charIndex);
        }

        c->dna = newDna;
        ui->dnaField->setText(newDna);
    } else {
        c->dna = ui->dnaField->text();
    }
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
}

void MainWindow::on_AGOT_checkBox_checkStateChanged(const Qt::CheckState &state)
{
    if (state == Qt::Checked){
        ui->inquisitive->setVisible(true);
        ui->authoritative->setVisible(true);
        ui->rude->setVisible(true);
    }
    else {
        ui->inquisitive->setVisible(false);
        ui->authoritative->setVisible(false);
        ui->rude->setVisible(false);
    }
}


void MainWindow::on_actionChange_character_index_triggered()
{
    if (characters.isEmpty()) {
        QMessageBox::warning(this, "No Active Character", "There is no active character to change the index of.");
        return;
    }

    CharacterData *activeCharacter = characters.last();

    // prompt for new index
    bool ok;
    int newNumber = QInputDialog::getInt(this, "Change Character Index", "Enter the new starting character number:", activeCharacter->characterNumber, 1, 100000, 1, &ok);

    if (!ok)
        return;

    activeCharacter->characterNumber = newNumber;

    updateDnaIfChecked(activeCharacter);

    // update ui line edits
    ui->nameField->setText(activeCharacter->name);
    ui->dnaField->setText(activeCharacter->dna);
    ui->religionField->setText(activeCharacter->religion);
    ui->cultureField->setText(activeCharacter->culture);
    ui->dynField->setText(activeCharacter->dynasty);
    ui->bdayField->setText(activeCharacter->birth);
    ui->deathdayField->setText(activeCharacter->death);

    ui->female_checkBox->setChecked(activeCharacter->gender == "female");
    ui->rtraits_checkBox->setChecked(activeCharacter->disallowRandomTraits);

    updateAllCharacterText();
    ui->openFileEdit_newchars->verticalScrollBar()->setValue(ui->openFileEdit_newchars->verticalScrollBar()->maximum());
}


void MainWindow::on_actionToggle_DNA_field_toggled(bool state)
{
    if (characters.isEmpty()) return;

    CharacterData *activeCharacter = characters.last();

    if (state == true){
        ui->dnaField->clear();
        activeCharacter->dna.clear();
        updateAllCharacterText();
        ui->dnaField->setVisible(false);
        ui->dna_checkBox->setChecked(false);
        ui->dna_checkBox->setVisible(false);
    } else {
        ui->dnaField->setVisible(true);
        ui->dna_checkBox->setVisible(true);
    }
}


void MainWindow::on_actionToggle_Death_date_field_toggled(bool state)
{
    if (characters.isEmpty()) return;

    CharacterData *activeCharacter = characters.last();

    if (state == true){
        ui->deathday->setVisible(false);
        ui->deathdayField->clear();
        activeCharacter->death.clear();
        updateAllCharacterText();
        ui->deathdayField->setVisible(false);
    } else {
        ui->deathday->setVisible(true);
        ui->deathdayField->setVisible(true);
    }
}


void MainWindow::on_actionReload_Text_not_file_triggered()
{
    initializeCharacterSheet();
}


void MainWindow::on_openFileEdit_modificationChanged(bool changed)
{
    if (changed == true) setWindowTitle("*" + currentFile);
}

