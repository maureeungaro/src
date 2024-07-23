// gui
#include "gui.h"

// gemc
#include "gutilities.h"

GemcGUI::GemcGUI([[maybe_unused]] string qtResourceFile, GOptions *gopts, EventDispenser *ed, QWidget *parent) :
        QWidget(parent),
        GStateMessage(gopts, "GemcGUI ", "verbosity"),  // GStateMessage derived
        eventDispenser(ed) {

    createLeftButtons();        // instantiates leftButtons
    createRightContent(gopts);  // instantiates rightContent: g4control

    // top rows button
    QHBoxLayout *topLayout = new QHBoxLayout;
    createTopButtons(topLayout);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(leftButtons);
    bottomLayout->addWidget(rightContent);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("GEMC: Geant4 Monte-Carlo"));

    // setting timer
    gtimer = new QTimer(this);
    connect(gtimer, SIGNAL(timeout()), this, SLOT(cycleBeamOn()));
}


void GemcGUI::updateGui() {
    vector <string> sBefore = gutilities::getStringVectorFromString(eventNumberLabel->text().toStdString());

    int nBefore = stoi(sBefore[2]);
    int nThatWasRun = nEvents->text().toInt();

    QString newNEvents("Event Number: ");
    newNEvents.append(std::to_string(nBefore + nThatWasRun).c_str());

    eventNumberLabel->setText(newNEvents);
}


GemcGUI::~GemcGUI() {
    delete leftButtons;
    delete rightContent;
    delete nEvents;
    delete eventNumberLabel;
}
