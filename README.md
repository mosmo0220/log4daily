### README.md
# log4daily

### Project Overview

**log4daily** is a work-in-progress application with the goal of providing a robust platform for task planning, milestone tracking, and journaling. The application aims to leverage AI for insightful analysis and reporting, helping users stay organized, track progress, and gain valuable insights into their daily activities and accomplishments.

**Please note:** This application is still under development, and the features outlined above are intended goals rather than completed functionalities.

### Installation

To install **log4daily**, follow these steps:

1. **Clone the repository:**
    ```sh
    git clone https://github.com/zaruuwa/log4daily.git
    cd log4daily
    ```

2. **Set the appropriate permissions:**

    For Linux and macOS:
    ```sh
    chmod +x install.sh
    ```

    For Windows:
    ```sh
    icacls install.sh /grant Everyone:F
    ```

3. **Run the installation script:**
    ```sh
    ./install.sh
    ```

This script will make binary file, copy it to app folder and add to console path
**Note**: At first use u must run app as root/sudo for creating configuration folder phase

### Update
To update **log4daily** to the latest version, follow these steps:

1. **Navigate to the project directory:**
    ```sh
    cd log4daily
    ```

2. **Pull the latest changes from the repository:**
    ```sh
    git pull
    ```

3. **Run the installation script again:**
    ```sh
    ./install.sh
    ```

This will update your local copy of **log4daily** with the latest changes and ensure that everything is set up correctly.

### Project Structure

The project is organized into several main folders to maintain clarity and modularity:

- **app**: The main directory containing source code and tests. Testing is performed using the **doctest** library to ensure code reliability and correctness.
- **docs**: A dedicated folder for project documentation.
- **include**: Contains essential libraries used within the application.
- **licenses**: Directory containing licenses for external libraries.

### License

This project is licensed under the **GNU Affero General Public License v3 (AGPL-3.0)**. For more information, refer to the [LICENSE](LICENSE) file.

### Used libraries

**FTXUI** for UI : License MIT
**nlohmann-json** for json serialization : License MIT
**doctest** for tests : License MIT

### Author

Created by **Marcin Osmolak-Rogaluk**.
