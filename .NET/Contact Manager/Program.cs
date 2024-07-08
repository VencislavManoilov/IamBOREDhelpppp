namespace ContactManager
{
    class Program
    {
        static List<Contact> contacts = [];

        static void Main(string[] args)
        {
            while(true) {
                Console.WriteLine("\n1. Add a contact");
                Console.WriteLine("2. View all contacts");
                Console.WriteLine("3. Delete a contact");
                Console.WriteLine("4. Exit");

                Console.WriteLine("Select a option: ");

                switch(Console.ReadLine()) {
                    case "1":
                        AddContact();
                    break;

                    case "2":
                        ViewContacts();
                    break;

                    case "3":
                        DeleteContact();
                    break;

                    case "4":
                    return;

                    default:
                        Console.WriteLine("Invalid option");
                    break;
                }
            }
        }

        static void AddContact() {
            string name;
            string phone;

            Console.WriteLine("\nName:");
            name = new(Console.ReadLine());
            
            Console.WriteLine("Phone:");
            phone = new(Console.ReadLine());

            contacts.Add(new Contact(name, phone));
        }

        static void ViewContacts() {
            Console.WriteLine("");
            foreach(Contact contact in contacts) {
                Console.WriteLine(contact.Name + " " + contact.Phone);
            }

            Console.WriteLine("Are you done? (yes)");
            string done = "";
            while(done != "yes") {
                done = new(Console.ReadLine());
            }
        }

        static void DeleteContact() {
            Console.WriteLine("Write the name of the contact to delete:");
            string name = new(Console.ReadLine());

            var contactToRemove = contacts.Find(c => c.Name.Equals(name, StringComparison.OrdinalIgnoreCase));
            if(contactToRemove.Equals != null) {
                contacts.Remove(contactToRemove);
                Console.WriteLine("Contact deleted successfully!");
            } else {
                Console.WriteLine("Contact not found!");
            }
        }

        struct Contact(string name, string phone)
        {
            public string Name { get; set; } = name;
            public string Phone { get; set; } = phone;
        }
    }
}