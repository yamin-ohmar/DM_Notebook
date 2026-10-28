
// Retrieve campaigns from the backend
async function loadCampaigns() {
    try {
        const response = await fetch("/api/campaigns");

        if (!response.ok) {
            throw new Error("Failed to retrieve campaigns");
        }

        const campaigns = await response.json();

        displayCampaigns(campaigns);

    } catch (error) {
        console.error("Error loading campaigns:", error);
        document.getElementById("campaign-list").textContent =
            "Unable to load campaigns.";
    }
}

// Display each campaign as a card
function displayCampaigns(campaigns) {
    const container = document.getElementById("campaign-list");

    // Clear previously displayed campaigns
    container.replaceChildren();

    if (campaigns.length === 0) {
        container.textContent = "No campaigns yet. Create your first adventure!";
        return;
    }

    campaigns.forEach(campaign => {
        const card = document.createElement("article");
        card.className = "campaign-card";

        const title = document.createElement("h3");
        title.textContent = campaign.name;

        const description = document.createElement("p");
        description.textContent = campaign.description || "No description yet.";

        // Edit and Delete buttons for this campaign
        const editButton = document.createElement("button");
        editButton.textContent = "Edit";
        editButton.className = "edit-btn";

        editButton.addEventListener("click", () => editCampaign(campaign));

        const deleteButton = document.createElement("button");
        deleteButton.textContent = "Delete";
        deleteButton.className = "delete-btn";

        deleteButton.addEventListener("click", () => deleteCampaign(campaign));

        card.append(title, description, editButton, deleteButton);
        container.appendChild(card);
    });
}


// Create a new campaign using the REST API
async function createCampaign(event) {
    // Prevent the browser from refreshing when the form is submitted
    event.preventDefault();

    const name = document.getElementById("campaign-name").value.trim();
    const description = document.getElementById("campaign-description").value.trim();
    const message = document.getElementById("campaign-form-message");

    // Validate the required campaign name
    if (!name) {
        message.textContent = "Please enter a campaign name.";
        return;
    }

    try {
        // Send the campaign information to Crow
        const response = await fetch("/api/campaigns", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                name: name,
                description: description
            })
        });

        if (!response.ok) {
            throw new Error("Failed to create campaign");
        }

        // Clear the form after successful creation
        document.getElementById("create-campaign-form").reset();

        message.textContent = "Campaign created successfully!";

        // Refresh the campaign cards
        await loadCampaigns();

    } catch (error) {
        console.error("Error creating campaign:", error);
        message.textContent = "Unable to create campaign.";
    }
}


async function editCampaign(campaign) {
    // Prompt the user for updated campaign details
    const draftName = prompt("Enter campaign name:", campaign.name);
    if (draftName === null) return;  // Cancel editing

    const name = draftName.trim();
    if (!name) return;

    // Prompt for optional details (Cancel aborts editing)
    const description = prompt("Enter a brief description:", campaign.description);
    if (description === null) return;

    // Create an updatedCampaign object
    const updatedCampaign = {
        name: name,
        description: description.trim()
    };

    try {
        // Send a PUT request
        const response = await fetch(`/api/campaigns/${campaign.id}`, {
            method: "PUT",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(updatedCampaign)    // converts JavaScript object into JSON text
        });

        // Throw an Error if the request was unsuccessful
        if (!response.ok) {
            throw new Error("Failed to update a campaign");
        }

        // Reload the campaign list
        await loadCampaigns();

    } catch (error) {
        console.error("Failed to update campaign:", error);
        alert("Unable to update campaign. Please try again.");
    }
}


async function deleteCampaign(campaign) {
    // Ask the user to confirm deletion
    const confirmed = confirm(
        `Are you sure you want to delete ${campaign.name}? This cannot be undone.`
    );

    // Stop if the user clicks Cancel
    if (!confirmed) return;

    try {
        // Send DELETE request
        // For delete, no JSON body to parse
        const response = await fetch(`/api/campaigns/${campaign.id}`, {
            method: "DELETE"
        });

        // Throw an Error if the request was unsuccessful
        if (!response.ok) {
            throw new Error("Failed to delete a campaign");
        }

        // Reload campaigns
        await loadCampaigns();

    } catch (error) {
        console.error("Failed to delete campaign:", error);
        alert("Unable to delete campaign. Please try again.");
    }
}


// Load campaigns when the page opens
document.addEventListener("DOMContentLoaded", () => {
    // Display existing campaigns
    loadCampaigns();

    // Handle form submission
    document.getElementById("create-campaign-form").addEventListener("submit", createCampaign);
});
