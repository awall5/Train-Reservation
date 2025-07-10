#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

// Class to store Train details
class Train {
private:
    int trainNo;
    char trainName[60], startingPoint[60], destination[60];
    int acFirstClassSeats, acSecondClassSeats, firstClassSleeperSeats, acChairCarSeats, secondClassSleeperSeats;
public:
    Train() {
        trainNo = acFirstClassSeats = acSecondClassSeats = firstClassSleeperSeats = acChairCarSeats = secondClassSleeperSeats = 0;
        strcpy(trainName, "");
        strcpy(startingPoint, "");
        strcpy(destination, "");
    }

    void input();              // Input train details
    void display() const;      // Display train details
    int getTrainNo() const { return trainNo; }

    // Getters for seat counts
    int getSeats(int choice) const {
        switch (choice) {
            case 1: return acFirstClassSeats;
            case 2: return acSecondClassSeats;
            case 3: return firstClassSleeperSeats;
            case 4: return acChairCarSeats;
            case 5: return secondClassSleeperSeats;
            default: return 0;
        }
    }

    // Reduce seat count
    void bookSeat(int choice) {
        switch (choice) {
            case 1: acFirstClassSeats--; break;
            case 2: acSecondClassSeats--; break;
            case 3: firstClassSleeperSeats--; break;
            case 4: acChairCarSeats--; break;
            case 5: secondClassSleeperSeats--; break;
        }
    }
};

// Input train details
void Train::input() {
    cout << "\nEnter Train Number: "; cin >> trainNo;
    cout << "Enter Train Name: "; cin.ignore(); cin.getline(trainName, 60);
    cout << "Enter Starting Point: "; cin.getline(startingPoint, 60);
    cout << "Enter Destination: "; cin.getline(destination, 60);
    cout << "Number of A/C First Class Seats: "; cin >> acFirstClassSeats;
    cout << "Number of A/C Second Class Seats: "; cin >> acSecondClassSeats;
    cout << "Number of First Class Sleeper Seats: "; cin >> firstClassSleeperSeats;
    cout << "Number of A/C Chair Car Seats: "; cin >> acChairCarSeats;
    cout << "Number of Second Class Sleeper Seats: "; cin >> secondClassSleeperSeats;
}

// Display train details
void Train::display() const {
    cout << "\n=== Train Details ==="
         << "\nTrain Number: " << trainNo
         << "\nTrain Name: " << trainName
         << "\nStarting Point: " << startingPoint
         << "\nDestination: " << destination
         << "\nA/C First Class Seats: " << acFirstClassSeats
         << "\nA/C Second Class Seats: " << acSecondClassSeats
         << "\nFirst Class Sleeper Seats: " << firstClassSleeperSeats
         << "\nA/C Chair Car Seats: " << acChairCarSeats
         << "\nSecond Class Sleeper Seats: " << secondClassSleeperSeats << endl;
}

// Class to store Ticket details
class Ticket {
private:
    int reservationNo, age, bookedTrainNo;
    char passengerName[40], status[20], seatClass[40];
public:
    Ticket() {
        reservationNo = age = bookedTrainNo = 0;
        strcpy(passengerName, "");
        strcpy(status, "");
        strcpy(seatClass, "");
    }

    void create(int trainNo, int choice);     // Create ticket if seat available
    void display(int resNo) const;            // Display ticket details if resNo matches
    int getReservationNo() const { return reservationNo; }
};

// Create ticket with selected class
void Ticket::create(int trainNo, int choice) {
    cout << "\nEnter Passenger Name: "; cin.ignore(); cin.getline(passengerName, 40);
    cout << "Enter Age: "; cin >> age;

    switch (choice) {
        case 1: strcpy(seatClass, "A/C First Class"); break;
        case 2: strcpy(seatClass, "A/C Second Class"); break;
        case 3: strcpy(seatClass, "First Class Sleeper"); break;
        case 4: strcpy(seatClass, "A/C Chair Car"); break;
        case 5: strcpy(seatClass, "Second Class Sleeper"); break;
        default: strcpy(seatClass, "Unknown"); break;
    }

    reservationNo = rand() % 10000 + 1000;
    strcpy(status, "Confirmed");
    bookedTrainNo = trainNo;

    // Save ticket to file
    ofstream fout("tickets.dat", ios::app | ios::binary);
    fout.write(reinterpret_cast<char*>(this), sizeof(*this));
    fout.close();

    cout << "\nTicket Reserved Successfully!"
         << "\nReservation Number: " << reservationNo
         << "\nSeat Class: " << seatClass
         << "\nStatus: " << status << endl;
}

// Display ticket details
void Ticket::display(int resNo) const {
    if (reservationNo == resNo) {
        cout << "\n=== Ticket Details ==="
             << "\nPassenger Name: " << passengerName
             << "\nAge: " << age
             << "\nReservation Number: " << reservationNo
             << "\nBooked Train Number: " << bookedTrainNo
             << "\nSeat Class: " << seatClass
             << "\nStatus: " << status << endl;
    }
}

int main() {
    int choice;

    while (true) {
        cout << "\n=== Railway Reservation Menu ==="
             << "\n1. Add Train Details"
             << "\n2. Show All Train Details"
             << "\n3. Reserve Ticket"
             << "\n4. Cancel Ticket"
             << "\n5. Show Ticket Status"
             << "\n6. Exit"
             << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            Train t;
            t.input();
            ofstream fout("trains.dat", ios::app | ios::binary);
            fout.write(reinterpret_cast<char*>(&t), sizeof(t));
            fout.close();
            cout << "Train added successfully.\n";
        }
        else if (choice == 2) {
            ifstream fin("trains.dat", ios::binary);
            Train t;
            bool any = false;
            while (fin.read(reinterpret_cast<char*>(&t), sizeof(t))) {
                t.display();
                any = true;
            }
            fin.close();
            if (!any) cout << "No train data available.\n";
        }
        else if (choice == 3) {
            int trainNo;
            cout << "Enter Train Number to Book: ";
            cin >> trainNo;

            ifstream fin("trains.dat", ios::binary);
            ofstream fout("temp.dat", ios::binary);
            Train t;
            bool found = false, booked = false;

            while (fin.read(reinterpret_cast<char*>(&t), sizeof(t))) {
                if (t.getTrainNo() == trainNo) {
                    found = true;
                    cout << "\nSelect Seat Class:"
                         << "\n1. A/C First Class"
                         << "\n2. A/C Second Class"
                         << "\n3. First Class Sleeper"
                         << "\n4. A/C Chair Car"
                         << "\n5. Second Class Sleeper"
                         << "\nEnter choice (1-5): ";
                    int seatChoice; cin >> seatChoice;

                    if (t.getSeats(seatChoice) > 0) {
                        t.bookSeat(seatChoice); // decrement seat count
                        Ticket ticket;
                        ticket.create(trainNo, seatChoice);
                        booked = true;
                    } else {
                        cout << "No seats available in selected class!\n";
                    }
                }
                fout.write(reinterpret_cast<char*>(&t), sizeof(t));
            }
            fin.close(); fout.close();

            // Replace old file with updated seat counts
            remove("trains.dat");
            rename("temp.dat", "trains.dat");

            if (!found) cout << "Invalid Train Number.\n";
        }
        else if (choice == 4) {
            int resNo;
            cout << "Enter Reservation Number to Cancel: ";
            cin >> resNo;

            ifstream fin("tickets.dat", ios::binary);
            ofstream fout("temp.dat", ios::binary);
            Ticket t;
            bool removed = false;

            while (fin.read(reinterpret_cast<char*>(&t), sizeof(t))) {
                if (t.getReservationNo() != resNo) {
                    fout.write(reinterpret_cast<char*>(&t), sizeof(t));
                } else {
                    removed = true;
                }
            }

            fin.close(); fout.close();
            remove("tickets.dat");
            rename("temp.dat", "tickets.dat");

            if (removed) cout << "Ticket cancelled successfully.\n";
            else cout << "Reservation number not found.\n";
        }
        else if (choice == 5) {
            int resNo;
            cout << "Enter Reservation Number to View: ";
            cin >> resNo;

            ifstream fin("tickets.dat", ios::binary);
            Ticket t;
            bool found = false;
            while (fin.read(reinterpret_cast<char*>(&t), sizeof(t))) {
                if (t.getReservationNo() == resNo) {
                    t.display(resNo);
                    found = true;
                    break;
                }
            }
            fin.close();
            if (!found) cout << "Reservation number not found.\n";
        }
        else if (choice == 6) {
            cout << "Thank you for using the system!\n";
            break;
        }
        else {
            cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}
