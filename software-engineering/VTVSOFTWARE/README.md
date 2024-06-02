# VTV Web Application

## Project Overview

The goal is to develop a web application for VTV, a television agency that records videos of current events and distributes them to its clients, which include journalists and news agencies. The application should allow cameramen to upload videos, editors to create TV services, and clients to search for content and receive notifications about new resources.

## Functional Requirements

### Cameramen
- **Video Uploads**: Cameramen upload videos to the platform. Each video is characterized by:
  - ID
  - Name
  - Recording date
  - Duration
  - Size

### Editors
- **TV Service Creation**: Editors create TV services by combining one or more videos. TV services can be:
  - Standard
  - Premium
  - Each service is associated with a current event.

- **Current Events**: Events can be defined by:
  - Cameramen independently from video uploads
  - Editors during the editing process if the event does not already exist on the platform.
  - Each event is identified by:
    - Name
    - Location
    - Description

### Clients
- **User Categories**:
  - **Standard Users**: Can only view standard TV services.
  - **Premium Users**: Can also purchase premium TV services.
  
- **User Information**: The platform stores the following information for all users:
  - First name
  - Last name
  - Date of birth
  - Address
  - Email address
  - **Premium Users**: Additionally, the subscription expiry date.

- **Content Search**:
  - Users can search for TV services based on a text search of the event name.
  - Standard users' search results exclude premium TV services.

- **Notification Service**:
  - Users can subscribe to receive notifications of new uploads.

### Business Intelligence
- **Marketing Department**: Requires data on the following:
  - **View Statistics**: Date of viewings by standard and premium users.
  - **Purchase Statistics**: Date and number of licenses purchased for TV services.

## Conclusion

This application aims to streamline the workflow for VTV's cameramen and editors, while providing a robust search and notification system for clients. Additionally, it will offer valuable insights for the marketing department through detailed viewing and purchasing statistics.
