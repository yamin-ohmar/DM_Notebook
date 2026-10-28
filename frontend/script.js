
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

        card.append(title, description);
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


// Load campaigns when the page opens
document.addEventListener("DOMContentLoaded", () => {
    // Display existing campaigns
    loadCampaigns();

    // Handle form submission
    document.getElementById("create-campaign-form").addEventListener("submit", createCampaign);
});
